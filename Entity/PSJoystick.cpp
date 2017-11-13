#include "PSJoystick.h"

#include "../Sparkler/Sparkler.h"
using namespace Sparkler;

#include "../InputKit.h"
#include "../GraphicsKit.h"

#include "../ResourceId.h"

const float LEVER_X = -3;
const float LEVER_Y = -2;
const float LEVER_Z = 0;
const float LEVER_TILT_MAX_ANGLE = Radian(30);
const float BUTTON_X = -1.5f;
const float BUTTON_Y = LEVER_Y + 0.2f;
const float BUTTON_Z = LEVER_Z;

PSJoystick::PSJoystick()
{
}

void PSJoystick::Update()
{
}

void PSJoystick::Render()
{
    float rotation = atan2(InputKit::stickY, InputKit::stickX);
    float tilt = Length(Vector(InputKit::stickY, InputKit::stickX, 0)) * -LEVER_TILT_MAX_ANGLE;

    Vector camera = Vector(0, 5, -7);
    Vector target = Vector(0, 0, 0);
    Vector up     = Vector(0, 1, 0);
    GraphicsKit::SetViewTransform(LookAtMatrix(camera, target, up));

    GraphicsKit::SetDirectionalLight(Vector(5, -5, 3), 0.8f, 1.5f);

    GraphicsKit::SetOrthogonalProjectionTransform();

    Matrix worldMatrix = RotationMatrixZ(tilt) * RotationMatrixY(-rotation) * TranslationMatrix(LEVER_X, LEVER_Y, LEVER_Z);
    GraphicsKit::RenderMesh(MESH_PS_ANALOG_LEVER, worldMatrix);
    worldMatrix = TranslationMatrix(LEVER_X, LEVER_Y, LEVER_Z);
    GraphicsKit::RenderMesh(MESH_PS_ANALOG_LEVER_BASE, worldMatrix);

    worldMatrix = TranslationMatrix(BUTTON_X, BUTTON_Y, BUTTON_Z);
    if (InputKit::fireButtonPressed) {
        GraphicsKit::RenderMesh(MESH_PS_BUTTON_PRESSED, worldMatrix);
    } else {
        GraphicsKit::RenderMesh(MESH_PS_BUTTON, worldMatrix);
    }
}


