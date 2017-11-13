#include <windows.h>

#include "Sparkler/Sparkler.h"
using namespace Sparkler;

#include "InputKit.h"
#include "SoundKit.h"
#include "GraphicsKit.h"

#include "resource.h"

#include "Game.h"
#include "ResourceId.h"

#include "Scene/SceneManager.h"
#include "Scene/TitleScene.h"
#include "Scene/StageScene.h"

#include "Sparkler/DetectMemoryLeaks.h"

const char* APPLICATION_NAME= "Delta Strider 1.31";

HWND hWnd;
bool windowed;
WINDOWPLACEMENT windowPlacement;

SceneManager* sceneManager;

Stopwatch* systemTimer;

const bool VSYNC = true;

//#define DEMO_RECORD_MODE

#define DEMO_RECORD_START_STAGE 13
#define DEMO_RECORD_START_LEFT	3
#define DEMO_RECORD_START_SCORE	0

void ToggleFullScreen()
{
    if (windowed) {
        GetWindowPlacement(hWnd, &windowPlacement);
        
        windowed = false;
        GraphicsKit::ToggleFullScreen();
    } else {
        windowed = true;
        GraphicsKit::ToggleFullScreen();
        
        SetWindowPlacement(hWnd, &windowPlacement);
    }
}

LRESULT CALLBACK MsgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case IDM_TOGGLEFULLSCREEN:
            ToggleFullScreen();
            return 0;
            
        case IDM_EXIT:
            SendMessage(hWnd, WM_CLOSE, 0, 0);
            return 0;
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_ENTERSIZEMOVE:
        systemTimer->Stop();
        break;

    case WM_EXITSIZEMOVE:
        systemTimer->Start();
        break;

    case WM_SETCURSOR:
        if (!windowed) {
            SetCursor(NULL);
            return 0;
        }
        break;

    //case WM_PAINT:
    //	Render();
    //	ValidateRect(hWnd, NULL);
    //	return 0;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    CoInitialize(NULL);

    _CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);

    Log::Open("DeltaStrider.log");

#ifdef _DEBUG
    windowed = true;
#else
    windowed = false;
#endif

    WNDCLASSEX wc;
    wc.cbSize			= sizeof(WNDCLASSEX); 
    wc.style			= CS_CLASSDC;
    wc.lpfnWndProc		= MsgProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, (LPCTSTR) IDI_DELTASTRIDER2);
    wc.hIconSm			= LoadIcon(hInstance, (LPCTSTR) IDI_DELTASTRIDER2);
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground	= (HBRUSH) GetStockObject(BLACK_BRUSH);
    wc.lpszMenuName		= (LPCTSTR) IDC_DELTASTRIDER2;
    wc.lpszClassName	= APPLICATION_NAME;
    RegisterClassEx(&wc);

    DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    
    RECT windowRect;
    windowRect.left		= 0;
    windowRect.top		= 0;
    windowRect.right	= Game::SCREEN_W;
    windowRect.bottom	= Game::SCREEN_H;
    AdjustWindowRect(&windowRect, style, true);

    int workAreaW = GetSystemMetrics(SM_CXSCREEN);
    int workAreaH = GetSystemMetrics(SM_CYSCREEN);

    int windowW = windowRect.right - windowRect.left;
    int windowH = windowRect.bottom - windowRect.top;
    int windowX = (workAreaW - windowW) / 2;
    int windowY = (workAreaH - windowH) / 2;
    hWnd = CreateWindow(APPLICATION_NAME, APPLICATION_NAME, style, 
                        windowX, windowY, windowW, windowH, 
                        GetDesktopWindow(), NULL, wc.hInstance, NULL);

    GetWindowPlacement(hWnd, &windowPlacement);

    HACCEL hAccel = LoadAccelerators(hInstance, (LPCTSTR) IDC_DELTASTRIDER2);

    try {
        SetCursor(LoadCursor(NULL, IDC_APPSTARTING));
        {
            InputKit::Initialize();
            SoundKit::Initialize(hWnd);
            GraphicsKit::Initialize(hWnd, Game::SCREEN_W, Game::SCREEN_H, windowed, VSYNC);

            GraphicsKit::LoadMesh(MESH_MYSHIP_TYPED, "MyShip TypeD.x");
            GraphicsKit::LoadMesh(MESH_PS_ANALOG_LEVER, "PS Analog Lever.x");
            GraphicsKit::LoadMesh(MESH_PS_ANALOG_LEVER_BASE, "PS Analog Lever Base.x");
            GraphicsKit::LoadMesh(MESH_PS_BUTTON, "PS Button.x");
            GraphicsKit::LoadMesh(MESH_PS_BUTTON_PRESSED, "PS Button Pressed.x");
            GraphicsKit::LoadMesh(MESH_ASTEROID, "Asteroid.x");
            GraphicsKit::LoadMesh(MESH_ASTEROID_DEBRIS, "Asteroid Debris.x");

            GraphicsKit::LoadMesh(MESH_BACKGROUND_0, "Background 0.x");
            GraphicsKit::LoadMesh(MESH_BACKGROUND_1, "Background 1.x");
            GraphicsKit::LoadMesh(MESH_BACKGROUND_2, "Background 2.x");
            GraphicsKit::LoadMesh(MESH_BACKGROUND_FRAME, "Background Frame.x");

            GraphicsKit::LoadMesh(MESH_BASE, "Base.x");
            GraphicsKit::LoadMesh(MESH_BASE_SHIELD, "Base Shield.x");

            GraphicsKit::LoadMesh(MESH_METAL_DEBRIS_1, "Metal Debris 1.x");
            GraphicsKit::LoadMesh(MESH_METAL_DEBRIS_2, "Metal Debris 2.x");

            GraphicsKit::LoadMesh(MESH_ENEMY_STRIDER_1, "Enemy Strider 1.x");

            GraphicsKit::LoadTexture(TEXTURE_TITLE, "title.png");
            GraphicsKit::LoadTexture(TEXTURE_OPTION, "option.png");
            GraphicsKit::LoadTexture(TEXTURE_CREDIT, "credit.png");

            GraphicsKit::LoadTexture(TEXTURE_OPTION_TEXT, "option_text.png");

            GraphicsKit::LoadTexture(TEXTURE_HUD, "hud.png");
            GraphicsKit::LoadTexture(TEXTURE_JET, "jet.png");
            GraphicsKit::LoadTexture(TEXTURE_MYSHOT, "myshot.png");
            GraphicsKit::LoadTexture(TEXTURE_EXPLOSION, "explosion.png");
            GraphicsKit::LoadTexture(TEXTURE_SMOKE, "smoke.png");
            GraphicsKit::LoadTexture(TEXTURE_MUZZLE_FLASH, "muzzle_flash.png");

            GraphicsKit::DefineSprite(SPRITE_TITLE, TEXTURE_TITLE, 0, 0, 640, 480);
            GraphicsKit::DefineSprite(SPRITE_OPTION, TEXTURE_OPTION, 0, 0, 640, 480);
            GraphicsKit::DefineSprite(SPRITE_CREDIT, TEXTURE_CREDIT, 0, 0, 640, 480);

            for (int i = 0; i < 3; i++) {
                GraphicsKit::DefineSprite(SPRITE_OPTION_TEXT_HARD - i, TEXTURE_OPTION_TEXT, 0, 3 + 19 * i, 72, 13);
            }
            for (int i = 3; i < 5; i++) {
                GraphicsKit::DefineSprite(SPRITE_OPTION_TEXT_EASY + i, TEXTURE_OPTION_TEXT, 0, 3 + 19 * i, 72, 13);
            }

            GraphicsKit::DefineSprite(SPRITE_CURSOR, TEXTURE_HUD, 0, 48, 37, 23, 37, 4);

            GraphicsKit::DefineSprite(SPRITE_JET, TEXTURE_JET, 0, 0, 128, 128, 160, 63);
            GraphicsKit::DefineSprite(SPRITE_MYSHOT_TYPED, TEXTURE_MYSHOT, 0, 0, 128, 128, 64, 64);
            GraphicsKit::DefineSprite(SPRITE_ENEMY_SHOT, TEXTURE_HUD, 256, 164, 101, 85, 50, 42);
            GraphicsKit::DefineSprite(SPRITE_SMOKE, TEXTURE_SMOKE, 1, 1, 126, 126, 64, 64);

            GraphicsKit::DefineSprite(SPRITE_HUD_SCORE, TEXTURE_HUD, 0, 0, 225, 48);
            GraphicsKit::DefineSprite(SPRITE_HUD_TIME_REMAINING, TEXTURE_HUD, 0, 164, 160, 26);
            GraphicsKit::DefineSprite(SPRITE_HUD_GAMEOVER, TEXTURE_HUD, 256, 0, 242, 56, 121, 28);

            for (int i = 0; i < 10; i++) {
                GraphicsKit::DefineSprite(SPRITE_HUD_SCORE_DIGIT + i, TEXTURE_HUD, i * 36, 96, 36, 36);
            }
            for (int i = 0; i < 10 + 2; i++) {
                GraphicsKit::DefineSprite(SPRITE_HUD_TIME_DIGIT + i, TEXTURE_HUD, i * 16, 132, 16, 16);
            }
            GraphicsKit::DefineSprite(SPRITE_HUD_TIME_ZERO, TEXTURE_HUD, 192, 132, 144, 16);
            GraphicsKit::DefineSprite(SPRITE_HUD_RADAR, TEXTURE_HUD, 0, 256, 180, 181);
            GraphicsKit::DefineSprite(SPRITE_HUD_RADAR_BLIP_MYSHIP, TEXTURE_HUD, 368, 188, 32, 32, 16, 12);
            GraphicsKit::DefineSprite(SPRITE_HUD_RADAR_BLIP_ENEMY, TEXTURE_HUD, 368 + 32, 188, 32, 32, 16, 16);
            GraphicsKit::DefineSprite(SPRITE_HUD_RADAR_BLIP_TARGET_ENEMY, TEXTURE_HUD, 368 + 64, 188, 32, 32, 16, 16);
            GraphicsKit::DefineSprite(SPRITE_HUD_STAGE_CLEAR_PANEL, TEXTURE_HUD, 200, 256, 275, 221, 137, 110);
            GraphicsKit::DefineSprite(SPRITE_HUD_STAGE, TEXTURE_HUD, 128, 55, 168, 30);
            
            for (int iy = 0; iy < 4; iy++) {
                for (int ix = 0; ix < 6; ix++) {
                    GraphicsKit::DefineSprite(
                        SPRITE_EXPLOSION + (iy * 6 + ix), TEXTURE_EXPLOSION, 
                        ix * 128 + 1, iy * 128 + 1, 126, 126, 64, 64);
                }
            }
            GraphicsKit::DefineSprite(SPRITE_FLASH, TEXTURE_EXPLOSION, 768, 0, 256, 256, 128, 128);

            for (int i = 0; i < 4; i++) {
                GraphicsKit::DefineSprite(
                    SPRITE_MUZZLE_FLASH + i, TEXTURE_MUZZLE_FLASH, 
                    i * 128 + 1, 0, 126, 126, 10, 64);
            }
        }
        SetCursor(LoadCursor(NULL, IDC_ARROW));
        ShowWindow(hWnd, SW_SHOW);

        SoundKit::LoadSE(SE_MYSHOT_TYPED, "myshot.wav");
        SoundKit::LoadSE(SE_MYSHIP_ADVENT, "myship_advent.wav");
        SoundKit::LoadSE(SE_HIT, "hit.wav");
        SoundKit::LoadSE(SE_HIT_KIKI, "hit_kiki.wav");
        SoundKit::LoadSE(SE_EXPLOSION, "explosion.wav");
        SoundKit::LoadSE(SE_EXPLOSION_BIG, "explosion_big.wav");
        SoundKit::LoadSE(SE_EXPLOSION_SMALL, "explosion_small.wav");
        SoundKit::LoadSE(SE_EXPLOSION_MYSHIP, "explosion_myship.wav");
        SoundKit::LoadSE(SE_CURSOR, "cursor.wav");
        SoundKit::LoadSE(SE_SELECT, "select.wav");
        SoundKit::LoadSE(SE_TIME_BONUS, "time_bonus.wav");
        SoundKit::LoadSE(SE_WARNING, "warning.wav");

        for (int i = 0; i <= Game::STAGE_MAX; i++) {
            Game::stageTime[i] = 3 * 60 * Game::INTERNAL_FPS;
        }
        Game::difficulty = Game::DIFFICULTY_NORMAL;
        Game::bgm = true;
        Game::exit = false;

#ifdef DEMO_RECORD_MODE
        InputKit::Record();
        Game::stage = DEMO_RECORD_START_STAGE;
        Game::left  = DEMO_RECORD_START_LEFT;;
        Game::score = DEMO_RECORD_START_SCORE;
        sceneManager = new SceneManager(new StageScene());
#else
        sceneManager = new SceneManager(new TitleScene(TitleScene::ITEM_GAME_START));
#endif

        systemTimer = new Stopwatch();
        systemTimer->Start();
        double internalTime = 0;
        double internalStep = 1.0 / Game::INTERNAL_FPS;
        const double INTERNAL_TIME_MAX_DELAY = 0.1;
        
        double currentSystemTime = systemTimer->GetTime();

        bool movie = false;
        
        while (!Game::exit) {
            MSG msg;
            while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT)
                    goto exit;

                if (!TranslateAccelerator(msg.hwnd, hAccel, &msg)) {
                    TranslateMessage(&msg);
                    DispatchMessage(&msg);
                }
            }

            if (GraphicsKit::IsDeviceLost()) {
                Sleep(100);
                continue;
            }

#if 1
            if (GetAsyncKeyState('R') & 0x8000) {
                GraphicsKit::SetRecording(true);
            }
#endif
            if (movie) {
                currentSystemTime += 1 / 60.0f;
            } else {
                if (!VSYNC) {
                    currentSystemTime += 1 / 60.0;
                    double waitTime = currentSystemTime - systemTimer->GetTime();
                    if (waitTime < 0) {
                        waitTime = 0;
                    }
                    if (waitTime > 0.1) {
                        waitTime = 0.1;
                    }
                    int waitTimeMillis = static_cast<int>(waitTime * 1000);
                    Log::Printf("time %d\n", waitTimeMillis);
                    Sleep(waitTimeMillis);
                } else {
                    currentSystemTime = systemTimer->GetTime();

                    if (internalTime + INTERNAL_TIME_MAX_DELAY < currentSystemTime) {
                        internalTime = currentSystemTime;
                    }
                }
            }

            while (internalTime <= currentSystemTime) {
                internalTime += internalStep;

                InputKit::Update();
                sceneManager->Update();
            }
            sceneManager->Render();
        }
exit:
        delete systemTimer;
        delete sceneManager;
    } catch (exception& e) {
        GraphicsKit::Cleanup();
        SoundKit::Cleanup();
        InputKit::Cleanup();

        MessageBox(hWnd, e.what(), "", MB_ICONERROR);
        Log::Close();

        CoUninitialize();
        return 0;
    }
    GraphicsKit::Cleanup();
    SoundKit::Cleanup();

#ifdef DEMO_RECORD_MODE
    InputKit::SaveSequence("replay/replay.dat");
#endif
    InputKit::Cleanup();
    
    Log::Close();

    CoUninitialize();
    return 0;
}


