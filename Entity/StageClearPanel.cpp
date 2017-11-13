#include <ctype.h>

#include "StageClearPanel.h"

#include "../Scene/StageScene.h"

StageClearPanel::StageClearPanel(StageScene* scene)
{
    this->scene = scene;

    SetState(STATE_SHOW_STAGE_CLEAR);
    timeBonus = 0;

    clearTime = Game::stageTime[Game::stage % Game::STAGE_MAX] - Game::timeRemaining;
}

void StageClearPanel::Update()
{
    GameObject::Update();
    
    switch (state) {
    case STATE_SHOW_STAGE_CLEAR:
        if (count > 200) {
            if (Game::timeRemaining > 0) {
                SetState(STATE_INCREASE_TIME_BONUS);
            } else {
                SetState(STATE_WAIT);
            }
        }
        break;
    
    case STATE_INCREASE_TIME_BONUS:
        {
            int speed = (InputKit::fireButtonPressed) ? 88 : 22;
            for (int i = 0; i < speed; i++) {
                if (Game::timeRemaining % 600 == 0) {
                    SoundKit::PlaySE(SE_TIME_BONUS);
                }
                if (Game::timeRemaining > 0) {
                    Game::timeRemaining--;
                    if (Game::timeRemaining % 5 == 0) {
                        timeBonus++;
                    }
                } else {
                    SetState(STATE_STOP_TIME_BONUS);
                }
            }
        }
        break;
        
    case STATE_STOP_TIME_BONUS:
        if (count > 500 || InputKit::fireButtonCount == 1) {
            SetState(STATE_ADD_SCORE);
        }
        break;
        
    case STATE_ADD_SCORE:
        SoundKit::PlaySE(SE_CURSOR);
        Game::score += timeBonus;
        timeBonus = 0;
        SetState(STATE_WAIT);
        break;
        
    case STATE_WAIT:
        if (count > 500) {
            scene->GotoNextStage();
            Die();
        }
        break;
    }
}

void StageClearPanel::Render()
{
    GraphicsKit::PutSprite(SPRITE_HUD_STAGE_CLEAR_PANEL, Game::SCREEN_W / 2, Game::SCREEN_H / 2);
    
    char buf[16];

    const float CLEAR_TIME_X = 286;
    const float CLEAR_TIME_Y = 222;

    int millisecond = (clearTime % Game::INTERNAL_FPS) * 1000 / Game::INTERNAL_FPS;
    int second = (clearTime / Game::INTERNAL_FPS) % 60;
    int minute = (clearTime / Game::INTERNAL_FPS) / 60;
    sprintf(buf, "%02d:%02d;%03d", minute, second, millisecond);
    for (int i = 0; i < 9; i++) {
        GraphicsKit::PutSprite(SPRITE_HUD_TIME_DIGIT + buf[i] - '0', CLEAR_TIME_X + i * 16, CLEAR_TIME_Y);
    }
    
    const float TIME_BONUS_X = 110;
    const float TIME_BONUS_Y = 285;
    sprintf(buf, "%9d", timeBonus);
    for (int i = 0; i < 9; i++) {
        if (isdigit(buf[i])) {
            GraphicsKit::PutSprite(SPRITE_HUD_SCORE_DIGIT + buf[i] - '0', TIME_BONUS_X + i * 36, TIME_BONUS_Y);
        }
    }
}


