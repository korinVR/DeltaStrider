#include "../Sparkler/Sparkler.h"
using namespace Sparkler;

#include "../GraphicsKit.h"
#include "../InputKit.h"
#include "../Game.h"
#include "../ResourceId.h"
#include "../Radar.h"

#include "../Entity/Asteroid.h"
#include "../Entity/Base.h"
#include "../Entity/Background.h"
#include "../Entity/EnemyStrider1.h"
#include "../Entity/MyShipTypeD.h"
#include "../Entity/PSJoystick.h"
#include "../Entity/StageCamera.h"
#include "../Entity/StageClearPanel.h"
#include "../Entity/StageText.h"

#include "../CollisionDetection/CollisionDetector.h"

#include "StageScene.h"
#include "GameOverScene.h"
#include "TitleScene.h"

StageScene::StageScene()
{
    Random::SetSeed(123456);

    int loop = (Game::stage - 1) / Game::STAGE_MAX;
    if (InputKit::Playing()) {
        Game::rank = Game::DIFFICULTY_NORMAL + loop;
    } else {
        Game::rank = Game::difficulty + loop;
    }
    if (Game::rank > 3) {
        Game::rank = 3;
    }

    Game::myShip = static_cast<MyShipPtr>(new MyShipTypeD());
    Game::camera = static_cast<CameraPtr>(new StageCamera());
    backgrounds.Clear();
    
    Game::myShots.Clear();
    Game::effects.Clear();
    Game::enemies.Clear();
    Game::targetEnemies.Clear();
    Game::enemyShots.Clear();

    Game::scoreCount = 0;
    
    if (InputKit::Playing()) {
        huds.Append(new PSJoystick());
    }

    stageCleared = false;
    gotoNextStage = false;

    Game::effects.Append(new StageText());

    st = ((Game::stage - 1) % Game::STAGE_MAX) + 1;
    Game::timeRemaining = Game::stageTime[st];
    backgrounds.Append(new Background((st - 1) / 5));

    switch (st) {
    case 1:
        if (Game::bgm) {
            SoundKit::PlayBGM("bgm1.ogg");
        }
        Game::targetEnemies.Append(new Base(Vector(30, 100, 0)));
        Game::targetEnemies.Append(new Base(Vector(100, 50, 0)));
        Game::targetEnemies.Append(new Base(Vector(-30, -100, 0)));
        Game::targetEnemies.Append(new Base(Vector(-100, -50, 0)));
        break;

    case 2:
        Game::targetEnemies.Append(new Base(Vector(0, 100, 0)));
        Game::targetEnemies.Append(new Base(Vector(-100, 100, 0)));
        Game::targetEnemies.Append(new Base(Vector(100, 100, 0)));

        Game::targetEnemies.Append(new Base(Vector(0, -100, 0)));
        Game::targetEnemies.Append(new Base(Vector(100, -100, 0)));
        break;

    case 3:
        Game::targetEnemies.Append(new Base(Vector(-10, 130, 0)));
        Game::targetEnemies.Append(new Base(Vector(-60, 110, 0)));
        Game::targetEnemies.Append(new Base(Vector(30, 120, 0)));

        Game::targetEnemies.Append(new Base(Vector(120, -50, 0)));
        Game::targetEnemies.Append(new Base(Vector(80, -70, 0)));

        Game::targetEnemies.Append(new Base(Vector(-160, -100, 0)));
        break;
    case 4:
        Game::targetEnemies.Append(new Base(Vector(-130, 40, 0)));
        Game::targetEnemies.Append(new Base(Vector(40, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(10, -150, 0)));
        Game::targetEnemies.Append(new Base(Vector(70, -120, 0)));
        Game::targetEnemies.Append(new Base(Vector(-90, -100, 0)));

        for (int i = 0; i < 150; i++) {
            Game::enemies.Append(new Asteroid());
        }
        break;

    case 5:
        Game::targetEnemies.Append(new Base(Vector(-100, -50, 0)));
        Game::targetEnemies.Append(new Base(Vector(-110, -100, 0)));
        Game::targetEnemies.Append(new Base(Vector(-120, -150, 0)));

        Game::targetEnemies.Append(new Base(Vector(100, 150, 0)));
        Game::targetEnemies.Append(new Base(Vector(110, 100, 0)));
        Game::targetEnemies.Append(new Base(Vector(120, 50, 0)));

        for (int i = 0; i < 30; i++) {
            Game::enemies.Append(new Asteroid());
        }
        break;

    case 6:
        if (Game::bgm) {
            SoundKit::PlayBGM("bgm2.ogg");
        }
        for (int i = 0; i < 5; i++) {
            Game::targetEnemies.Append(new Base(Vector(static_cast<float>(-100 + i * 35), static_cast<float>(20 + i * 15), 0)));
        }
        break;

    case 7:
        Game::targetEnemies.Append(new Base(Vector(-80, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(0, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(80, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(-40, -120, 0)));
        Game::targetEnemies.Append(new Base(Vector(20, 0, 0)));
        break;

    case 8:
        Game::targetEnemies.Append(new Base(Vector(-130, 130, 0)));
        Game::targetEnemies.Append(new Base(Vector(130, 130, 0)));
        Game::targetEnemies.Append(new Base(Vector(-140, -120, 0)));
        Game::targetEnemies.Append(new Base(Vector(130, -130, 0)));
        Game::targetEnemies.Append(new Base(Vector(140, 100, 0)));
        Game::targetEnemies.Append(new Base(Vector(100, -130, 0)));

        for (int i = 0; i < 200; i++) {
            Game::enemies.Append(new Asteroid());
        }
        break;

    case 9:
        Game::targetEnemies.Append(new Base(Vector(-100, 70, 0)));
        Game::targetEnemies.Append(new Base(Vector(-130, 100, 0)));
        Game::targetEnemies.Append(new Base(Vector(-70, 100, 0)));

        Game::targetEnemies.Append(new Base(Vector(100, -130, 0)));
        Game::targetEnemies.Append(new Base(Vector(100, -70, 0)));
        Game::targetEnemies.Append(new Base(Vector(130, -100, 0)));

        Game::targetEnemies.Append(new Base(Vector(-90, -90, 0)));
        break;

    case 10:
        for (int i = 0; i < 8; i++) {
            Vector position;

            position.x = Random::GetReal(-150, 150);
            position.y = Random::GetReal(-150, 150);
            Game::targetEnemies.Append(new Base(position));
        }

        for (int i = 0; i < 40; i++) {
            Game::enemies.Append(new Asteroid());
        }
        break;

    case 11:
        if (Game::bgm) {
            SoundKit::PlayBGM("bgm3.ogg");
        }
        Game::targetEnemies.Append(new Base(Vector(80, 0, 0)));
        Game::targetEnemies.Append(new Base(Vector(-80, 0, 0)));
        Game::targetEnemies.Append(new Base(Vector(0, 80, 0)));
        Game::targetEnemies.Append(new Base(Vector(0, -80, 0)));

        Game::targetEnemies.Append(new Base(Vector(40, 40, 0)));
        Game::targetEnemies.Append(new Base(Vector(-40, 40, 0)));
        Game::targetEnemies.Append(new Base(Vector(40, -40, 0)));
        Game::targetEnemies.Append(new Base(Vector(-40, -40, 0)));
        break;

    case 12:
        Game::targetEnemies.Append(new Base(Vector(-120, -120, 0)));
        Game::targetEnemies.Append(new Base(Vector(-120, 0, 0)));
        Game::targetEnemies.Append(new Base(Vector(-120, 120, 0)));

        Game::targetEnemies.Append(new Base(Vector(100, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(140, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(100, 80, 0)));
        Game::targetEnemies.Append(new Base(Vector(140, 80, 0)));
        break;

    case 13:
        Random::SetSeed(5);
        for (int i = 0; i < 10; i++) {
            Vector position;

            position.x = Random::GetReal(-170, 170);
            position.y = Random::GetReal(-170, 170);
            Game::targetEnemies.Append(new Base(position));
        }
        break;

    case 14:
        Game::targetEnemies.Append(new Base(Vector(120, 120, 0)));
        Game::targetEnemies.Append(new Base(Vector(0, 120, 0)));

        Game::targetEnemies.Append(new Base(Vector(-120, -120, 0)));
        Game::targetEnemies.Append(new Base(Vector(0, -120, 0)));

        for (int i = 0; i < 8; i++) {
            Vector position;

            position.x = cos(PI2 * i / 8) * 60;
            position.y = sin(PI2 * i / 8) * 60;
            Game::enemies.Append(new EnemyStrider1(position));
        }
        break;

    case 15:
        Random::SetSeed(45678);
        for (int i = 0; i < 12; i++) {
            Vector position;

            position.x = Random::GetReal(-150, 150);
            position.y = Random::GetReal(-150, 150);
            Game::targetEnemies.Append(new Base(position));
        }
        for (int i = 0; i < 300; i++) {
            Game::enemies.Append(new Asteroid());
        }
        break;
    }
}

void StageScene::Update(SceneManager* manager)
{
#if 0
    if (InputKit::statsButtonCount == 1) {
        gotoNextStage = true;
    }
    if (InputKit::viewButtonCount == 1) {
        Game::left = 0;
    }
#endif

    if (InputKit::aborted) {
        InputKit::Reset();
        InputKit::fireButtonCount = 1;
        SoundKit::StopBGM();
        ChangeScene(manager, new TitleScene(TitleScene::ITEM_GAME_START));
        return;
    }

    CollisionDetector::Clear();

    Game::myShip->Update();
    Game::camera->Update();
    backgrounds.Update();
    
    Game::effects.Update();
    Game::myShots.Update();
    Game::enemies.Update();
    Game::targetEnemies.Update();
    Game::enemyShots.Update();
    
    CollisionDetector::Detect();

    huds.Update();
    
    if (!stageCleared) {
        Game::timeRemaining--;
    }
    if (Game::scoreCount > 0) {
        Game::scoreCount--;
        Game::score++;
    }

    if (Game::left == 0) {
        if (Game::bgm) {
            SoundKit::StopBGM();
        }
        ChangeScene(manager, new GameOverScene());
    }
    switch (st) {
    case 2:
        if (manager->GetFrame() % 3000 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(-Game::OVERFIELD_SIZE, 0, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, 0, 0)));
        }
        break;

    case 3:
        if (manager->GetFrame() % 3000 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, 150, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(-80, -Game::OVERFIELD_SIZE, 0)));
        }
        break;

    case 4:
        if (manager->GetFrame() % 3000 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, 0, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(0, Game::OVERFIELD_SIZE, 0)));
        }
        break;

    case 5:
        if (manager->GetFrame() % 3000 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(-Game::OVERFIELD_SIZE, 150, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(-Game::OVERFIELD_SIZE, 100, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, -100, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, -150, 0)));
        }
        break;

    case 7:
        if (manager->GetFrame() % 1500 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(-Game::OVERFIELD_SIZE, 80, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, -80, 0)));
        }
        break;

    case 8:
        if (manager->GetFrame() % 5000 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(-Game::OVERFIELD_SIZE, Game::OVERFIELD_SIZE, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(-Game::OVERFIELD_SIZE, -Game::OVERFIELD_SIZE, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, Game::OVERFIELD_SIZE, 0)));
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, -Game::OVERFIELD_SIZE, 0)));
        }
        break;

    case 9:
        if (manager->GetFrame() % 500 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, Game::OVERFIELD_SIZE, 0)));
        }
        break;

    case 10:
        if (manager->GetFrame() % 1000 == 0) {
            float angle = Random::GetReal(0, PI2);
            float x = cos(angle) * Game::OVERFIELD_SIZE;
            float y = sin(angle) * Game::OVERFIELD_SIZE;
            Game::enemies.Append(new EnemyStrider1(Vector(x, y, 0)));
        }
        break;

    case 12:
        if (manager->GetFrame() % 1000 == 100) {
            Game::enemies.Append(new EnemyStrider1(Vector(Game::OVERFIELD_SIZE, -150, 0)));
        }
        break;

    case 13:
        if (manager->GetFrame() % 600 == 0) {
            float angle = Random::GetReal(0, PI2);
            float x = cos(angle) * Game::OVERFIELD_SIZE;
            float y = sin(angle) * Game::OVERFIELD_SIZE;
            Game::enemies.Append(new EnemyStrider1(Vector(x, y, 0)));
        }
        break;

    case 14:
        if (manager->GetFrame() % 2000 == 0) {
            for (int i = 0; i < 8; i++) {
                Vector position;

                position.x = cos(PI2 * i / 8) * Game::OVERFIELD_SIZE;
                position.y = sin(PI2 * i / 8) * Game::OVERFIELD_SIZE;
                Game::enemies.Append(new EnemyStrider1(position));
            }
        }
        break;

    case 15:
        if (manager->GetFrame() % 800 == 0) {
            float angle = Random::GetReal(0, PI2);
            float x = cos(angle) * Game::OVERFIELD_SIZE;
            float y = sin(angle) * Game::OVERFIELD_SIZE;
            Game::enemies.Append(new EnemyStrider1(Vector(x, y, 0)));
        }
        break;

    }

    if (Game::timeRemaining <= 0 && !stageCleared && (manager->GetFrame() % 3000 == 0)) {
        for (int i = 0; i < 6; i++) {
            Vector position;

            position.x = cos(PI2 * i / 6) * Game::OVERFIELD_SIZE;
            position.y = sin(PI2 * i / 6) * Game::OVERFIELD_SIZE;
            Game::enemies.Append(new EnemyStrider1(position));
        }
    }

    if (!stageCleared && Game::targetEnemies.GetSize() == 0) {
        stageCleared = true;
        Game::myShip->SetState(MyShip::STATE_STAGE_CLEAR);
        
        huds.Append(new StageClearPanel(this));
    }
    if (gotoNextStage) {
        Game::stage++;
        ChangeScene(manager, new StageScene());
    }

    if (Game::timeRemaining == -1) {
        SoundKit::PlaySE(SE_WARNING);
    }

    leftShipAngle = Norm(leftShipAngle + Radian(1));
}

void StageScene::Render(SceneManager* manager)
{
    GraphicsKit::Clear();
    
    Radar::RenderRadar();

    Game::camera->Render();

    backgrounds.Render();

    GraphicsKit::SetLighting(true);
    GraphicsKit::SetZTest(true, true);
    GraphicsKit::EnableFog(0x00000000, 70.0f, 90.0f);

    GraphicsKit::SetDirectionalLight(Vector(1, -1.5, 1), 1.0f, 1.0f);

    Game::myShip->Render();
    Game::myShots.Render();
    Game::enemies.Render();
    Game::targetEnemies.Render();
    Game::enemyShots.Render();

    GraphicsKit::DisableFog();
    Game::effects.Render();
    GraphicsKit::RenderEffects();

    GraphicsKit::SetLighting(true);
    GraphicsKit::SetZTest(false, false);
    GraphicsKit::SetAlpha(false);

    Vector camera = Vector(0, 0, -50);
    Vector target = Vector(0, 0, 0);
    Vector up = Vector(0, 1, 0);
    GraphicsKit::SetViewTransform(LookAtMatrix(camera, target, up));
    float aspectRatio = static_cast<float>(Game::SCREEN_W) / Game::SCREEN_H;
    GraphicsKit::SetProjectionTransform(PerspectiveMatrix(Radian(45), aspectRatio, 1, 200));

    for (int i = 0; i < Game::left - 1; i++) {
        Matrix matrix = ScalingMatrix(0.5f) * 
                        RotationMatrixX(leftShipAngle) * 
                        RotationMatrixZ(leftShipAngle) *
                        RotationMatrixY(leftShipAngle) * 
                        TranslationMatrix(-23.0f + 3 * i, -17.8f, 0);
        GraphicsKit::RenderMesh(MESH_MYSHIP_TYPED, matrix);
    }

    GraphicsKit::SetLighting(true);
    GraphicsKit::SetZTest(true, true);
    GraphicsKit::SetAlpha(false);
    huds.Render();

    const float HUD_SCORE_X = 40;
    const float HUD_SCORE_Y = 40;
    const float HUD_SCORE_VALUE_X = HUD_SCORE_X + 40;
    const float HUD_SCORE_VALUE_Y = HUD_SCORE_Y + 48;
    const float HUD_TIME_X = 464;
    const float HUD_TIME_Y = HUD_SCORE_Y;
    const float HUD_TIME_VALUE_X = HUD_TIME_X + 7;
    const float HUD_TIME_VALUE_Y = HUD_TIME_Y + 24;

    GraphicsKit::PutSprite(SPRITE_HUD_SCORE, HUD_SCORE_X, HUD_SCORE_Y);
    char buf[16];
    sprintf(buf, "%-9d", Game::score);
    for (int i = 0; i < 9; i++) {
        if ('0' <= buf[i] && buf[i] <= '9') {
            GraphicsKit::PutSprite(SPRITE_HUD_SCORE_DIGIT + buf[i] - '0', HUD_SCORE_VALUE_X + i * 36, HUD_SCORE_VALUE_Y);
        }
    }

    if (stageCleared || Game::timeRemaining > 30 * Game::INTERNAL_FPS || abs(Game::timeRemaining % 100 < 50)) {
        GraphicsKit::PutSprite(SPRITE_HUD_TIME_REMAINING, HUD_TIME_X, HUD_TIME_Y);
    }
    if (Game::timeRemaining > 0) {
        int millisecond = (Game::timeRemaining % Game::INTERNAL_FPS) * 1000 / Game::INTERNAL_FPS;
        int second = (Game::timeRemaining / Game::INTERNAL_FPS) % 60;
        int minute = (Game::timeRemaining / Game::INTERNAL_FPS) / 60;
        sprintf(buf, "%02d:%02d;%03d", minute, second, millisecond);
        for (int i = 0; i < 9; i++) {
            GraphicsKit::PutSprite(SPRITE_HUD_TIME_DIGIT + buf[i] - '0', HUD_TIME_VALUE_X + i * 16, HUD_TIME_VALUE_Y);
        }
    } else {
        GraphicsKit::PutSprite(SPRITE_HUD_TIME_ZERO, HUD_TIME_VALUE_X, HUD_TIME_VALUE_Y);
    }

    GraphicsKit::RenderSprites();

    if (InputKit::Playing()) {
        if (manager->GetFrame() % 200 > 80) {
            GraphicsKit::Printf(250, 350, "DEMONSTRATION");
        }
    }

    GraphicsKit::Present();
}

void StageScene::GotoNextStage()
{
    gotoNextStage = true;
}


