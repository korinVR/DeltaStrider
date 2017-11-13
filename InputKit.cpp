#include <windows.h>

#include <cstdio>
#include <cmath>
using namespace std;

#include <boost/cstdint.hpp>
using namespace boost;

#include "Sparkler/Sparkler.h"
using namespace Sparkler;

#include "InputKit.h"
#include "SoundKit.h"

#include "ResourceId.h"

float InputKit::stickX;
float InputKit::stickY;

int InputKit::stickUpCount;
int InputKit::stickDownCount;
int InputKit::stickLeftCount;
int InputKit::stickRightCount;

bool InputKit::fireButtonPressed;
bool InputKit::statsButtonPressed;
bool InputKit::viewButtonPressed;

int InputKit::fireButtonCount;
int InputKit::statsButtonCount;
int InputKit::viewButtonCount;

bool InputKit::aborted;

const float AXIS_ALLOWANCE = 0.15f;
const float CENTER_ALLOWANCE = 0.2f;
const float STICK_CLICK_THRESHOLD = 0.5f;

namespace {
    bool canUseJoystick = false;
    JOYCAPS joyCaps;

    bool recording = false;
    bool playing = false;

    const uint32_t SEQUENCE_MAX_LENGTH = 300 * 180;

    struct SequencePacket {
        float stickX;
        float stickY;
        bool fireButtonPressed;
    } sequence[SEQUENCE_MAX_LENGTH];

    uint32_t sequenceLength;
    uint32_t sequencePosition;
}

void InputKit::Initialize()
{
    if (joyGetDevCaps(JOYSTICKID1, &joyCaps, sizeof(JOYCAPS)) == JOYERR_NOERROR) {
        canUseJoystick = true;
    }
    Reset();
}

void InputKit::Cleanup()
{
}

void InputKit::Reset()
{
    stickX = 0.0f;
    stickY = 0.0f;
    
    stickUpCount = 0;
    stickDownCount = 0;
    stickLeftCount = 0;
    stickRightCount = 0;
    
    fireButtonPressed = false;
    statsButtonPressed = false;
    viewButtonPressed = false;
    
    fireButtonCount = 0;
    statsButtonCount = 0;
    viewButtonCount = 0;

    playing = false;
    recording = false;
    aborted = false;
}

void InputKit::Update()
{
    fireButtonPressed = false;
    statsButtonPressed = false;
    viewButtonPressed = false;
    int key = 5;

    if ((GetAsyncKeyState(VK_UP) | GetAsyncKeyState(VK_NUMPAD8)) & 0x8000) {
        key += 3;
    } else if ((GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState(VK_NUMPAD2)) & 0x8000) {
        key -= 3;
    }
    if ((GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState(VK_NUMPAD6)) & 0x8000) {
        key++;
    } else if ((GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState(VK_NUMPAD4)) & 0x8000) {
        key--;
    }
    if (GetAsyncKeyState(VK_NUMPAD1) & 0x8000) { key = 1; }
    if (GetAsyncKeyState(VK_NUMPAD3) & 0x8000) { key = 3; }
    if (GetAsyncKeyState(VK_NUMPAD7) & 0x8000) { key = 7; }
    if (GetAsyncKeyState(VK_NUMPAD9) & 0x8000) { key = 9; }

    if ((GetAsyncKeyState(VK_LCONTROL) | GetAsyncKeyState('Z') | GetAsyncKeyState(VK_SPACE)) & 0x8000) {
        fireButtonPressed = true;
    }
    if (GetAsyncKeyState('S') & 0x8000) {
        statsButtonPressed = true;
    }
    if (GetAsyncKeyState('V') & 0x8000) {
        viewButtonPressed = true;
    }

    const float O = 0.0f;
    const float I = 1.0f;
    const float S = 0.7071068f;
    const float x[] = { O, -S, O, S, -I, O, I, -S, O, S };
    const float y[] = { O, S, I, S, 0, 0, 0, -S, -I, -S };
    stickX = x[key];
    stickY = y[key];

    if (canUseJoystick) {
        JOYINFO joyInfo;
        joyGetPos(JOYSTICKID1, &joyInfo);
        if (key == 5) {
            stickX = (joyInfo.wXpos - joyCaps.wXmin) * 2.0f / (joyCaps.wXmax - joyCaps.wXmin) - 1.0f;
            stickY = (joyInfo.wYpos - joyCaps.wYmin) * 2.0f / (joyCaps.wYmax - joyCaps.wYmin) - 1.0f;

            if (-AXIS_ALLOWANCE < stickX && stickX < AXIS_ALLOWANCE) { stickX = 0.0f; }
            if (-AXIS_ALLOWANCE < stickY && stickY < AXIS_ALLOWANCE) { stickY = 0.0f; }

            if (Square(stickX) + Square(stickY) < Square(CENTER_ALLOWANCE)) {
                stickX = 0.0f;
                stickY = 0.0f;
            }

            if (Square(stickX) + Square(stickY) > 1.0f) {
                float direction = atan2(stickY, stickX);
                stickX = cos(direction);
                stickY = sin(direction);
            }
        }
        if (joyInfo.wButtons & (JOY_BUTTON1 | JOY_BUTTON2 | JOY_BUTTON3 | JOY_BUTTON4)) {
            fireButtonPressed = true;
        }
    }
    stickY = -stickY;

    if (playing) {
        if (sequencePosition >= sequenceLength) {
            playing = false;
            aborted = true;
        } else {
            if (fireButtonPressed) {
                aborted = true;

                SoundKit::PlaySE(SE_SELECT);
            }
            stickX = sequence[sequencePosition].stickX;
            stickY = sequence[sequencePosition].stickY;
            fireButtonPressed = sequence[sequencePosition].fireButtonPressed;
            sequencePosition++;
        }
    }
    if (recording) {
        if (sequencePosition >= SEQUENCE_MAX_LENGTH) {
            recording = false;
        } else {
            sequence[sequencePosition].stickX = stickX;
            sequence[sequencePosition].stickY = stickY;
            sequence[sequencePosition].fireButtonPressed = fireButtonPressed;
            sequencePosition++;
        }
    }

    if (stickY >  STICK_CLICK_THRESHOLD) { stickUpCount++; } else { stickUpCount = 0; }
    if (stickY < -STICK_CLICK_THRESHOLD) { stickDownCount++; } else { stickDownCount = 0; }
    if (stickX < -STICK_CLICK_THRESHOLD) { stickLeftCount++; } else { stickLeftCount = 0; }
    if (stickX >  STICK_CLICK_THRESHOLD) { stickRightCount++; } else { stickRightCount = 0; }

    if (fireButtonPressed) { fireButtonCount++; } else { fireButtonCount = 0; }
    if (statsButtonPressed) { statsButtonCount++; } else { statsButtonCount = 0; }
    if (viewButtonPressed) { viewButtonCount++; } else { viewButtonCount = 0; }
}

void InputKit::Record()
{
    Reset();
    sequencePosition = 0;
    recording = true;
    playing = false;
}

void InputKit::Play()
{
    Reset();
    sequencePosition = 0;
    recording = false;
    playing = true;
    aborted = false;
}

bool InputKit::Playing()
{
    return playing;
}

void InputKit::LoadSequence(char* filename)
{
    FILE* fp = fopen(filename, "rb");
    fread(&sequenceLength, sizeof(sequenceLength), 1, fp);
    fread(sequence, sizeof(SequencePacket), sequenceLength, fp);
    fclose(fp);
}

void InputKit::SaveSequence(char* filename)
{
    sequenceLength = sequencePosition;

    FILE* fp = fopen(filename, "wb");
    fwrite(&sequenceLength, sizeof(sequenceLength), 1, fp);
    fwrite(sequence, sizeof(SequencePacket), sequenceLength, fp);
    fclose(fp);
}

