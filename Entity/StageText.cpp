#include "StageText.h"

#include "../Game.h"
#include "../ResourceId.h"

StageText::StageText()
{
    count = 0;
    alpha = 0;
    scale = 1;
}

void StageText::Update()
{
    count++;

    if (150 < count && count < 1000) {
        if (alpha < 255) {
            alpha++;
        }
    }

    if (count > 1800) {
        alpha -= 2;
        scale *= 1.05f;
        if (alpha <= 0) {
            Die();
        }
    }
}

void StageText::Render()
{
    const int STAGE_TEXT_WIDTH = 160;
    const int BLANK_WIDTH = 16;
    const int DIGIT_WIDTH = 32;
    const int HEIGHT = 32;
    const int FIGURE = 2;

    int width = STAGE_TEXT_WIDTH + BLANK_WIDTH + DIGIT_WIDTH * FIGURE;
    int height = HEIGHT;

    float x = 320 - width / 2 * scale;
    float y = 240 - height / 2 * scale;

    GraphicsKit::PutSprite(SPRITE_HUD_STAGE, x, y, scale, 0, alpha);

    char buf[16];
    sprintf(buf, "%2d", Game::stage);
    for (int i = 0; i < FIGURE; i++) {
        if (isdigit(buf[i])) {
            GraphicsKit::PutSprite(SPRITE_HUD_SCORE_DIGIT + buf[i] - '0', 
                x + static_cast<int>((STAGE_TEXT_WIDTH + BLANK_WIDTH + DIGIT_WIDTH * i) * scale), y, scale, 0, alpha);
        }
    }
}


