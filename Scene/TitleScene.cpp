#include "TitleScene.h"

#include "../InputKit.h"
#include "../SoundKit.h"
#include "../GraphicsKit.h"

#include "StageScene.h"
#include "OptionScene.h"
#include "CreditScene.h"

#include "../Game.h"
#include "../ResourceId.h"

TitleScene::TitleScene(int item) : item(item)
{
    if (item == ITEM_GAME_START) {
        alpha = 0;
    } else {
        alpha = 255;
    }

    started = false;
    count = 0;

    startDemoCount = 0;
}

void TitleScene::Update(SceneManager* manager)
{
    if (alpha < 255) {
        alpha++;
    }

    if (started) {
        if (++count >= 400) {
            Game::stage = 1;
            Game::left = 3;
            Game::score = 0;
            ChangeScene(manager, new StageScene());
        }
    } else {
        if (++startDemoCount >= START_DEMO_TIME) {
            if (++Game::demo > Game::DEMO_NUM) {
                Game::demo = 1;
            }
            switch (Game::demo) {
            case 1:
                InputKit::LoadSequence("replay/demo1.dat");
                Game::stage = 1;
                Game::left = 3;
                Game::score = 0;
                break;

            case 2:
                if (Game::bgm) {
                    SoundKit::PlayBGM("bgm2.ogg");
                }
                InputKit::LoadSequence("replay/demo2.dat");
                Game::stage = 9;
                Game::left = 3;
                Game::score = 0;
                break;

            case 3:
                if (Game::bgm) {
                    SoundKit::PlayBGM("bgm3.ogg");
                }
                InputKit::LoadSequence("replay/demo3.dat");
                Game::stage = 13;
                Game::left = 3;
                Game::score = 0;
                break;
            }
            InputKit::Play();
            ChangeScene(manager, new StageScene());
            return;
        }

        if (InputKit::stickUpCount == 1 && item > 0) {
            item--;
            SoundKit::PlaySE(SE_CURSOR);
            startDemoCount = 0;
        }
        if (InputKit::stickDownCount == 1 && item < ITEM_MAX) {
            item++;
            SoundKit::PlaySE(SE_CURSOR);
            startDemoCount = 0;
        }

        if (InputKit::fireButtonCount == 1) {
            SoundKit::PlaySE(SE_SELECT);

            switch (item) {
            case ITEM_GAME_START:
                started = true;
                break;

            case ITEM_OPTION:
                ChangeScene(manager, new OptionScene());
                break;

            case ITEM_CREDIT:
                ChangeScene(manager, new CreditScene());
                break;

            case ITEM_EXIT:
                Game::exit = true;
                break;
            }
        }
    }
}

void TitleScene::Render(SceneManager* manager)
{
    const float CURSOR_X = 174;
    const float CURSOR_Y = 288;
    const float CURSOR_DY = 22;

    GraphicsKit::Clear();
    GraphicsKit::Fill(0, 0, 0);
    GraphicsKit::PutSprite(SPRITE_TITLE, 0, 0, 1, 0, alpha);
    if (!started || count % 80 > 40) {
        GraphicsKit::PutSprite(SPRITE_CURSOR, CURSOR_X, CURSOR_Y + item * CURSOR_DY, 1, 0, alpha);
    }
    GraphicsKit::RenderSprites();
    GraphicsKit::Present();
}


