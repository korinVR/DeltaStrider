#include "GameOverScene.h"

#include "../InputKit.h"
#include "../SoundKit.h"
#include "../GraphicsKit.h"

#include "StageScene.h"
#include "TitleScene.h"

#include "../Game.h"
#include "../ResourceId.h"

GameOverScene::GameOverScene()
{
	count = 0;
}

void GameOverScene::Update(SceneManager* manager)
{
	if (++count > 900) {
		ChangeScene(manager, new TitleScene(TitleScene::ITEM_GAME_START));
	}
}

void GameOverScene::Render(SceneManager* manager)
{
	GraphicsKit::Clear();
	GraphicsKit::Fill(0, 0, 0);
	GraphicsKit::PutSprite(SPRITE_HUD_GAMEOVER, 320, 240);

	const float HUD_SCORE_X = 40;
	const float HUD_SCORE_Y = 40;
	const float HUD_SCORE_VALUE_X = HUD_SCORE_X + 40;
	const float HUD_SCORE_VALUE_Y = HUD_SCORE_Y + 48;

	GraphicsKit::PutSprite(SPRITE_HUD_SCORE, HUD_SCORE_X, HUD_SCORE_Y);
	char buf[16];
	sprintf(buf, "%-9d", Game::score);
	for (int i = 0; i < 9; i++) {
		if ('0' <= buf[i] && buf[i] <= '9') {
			GraphicsKit::PutSprite(SPRITE_HUD_SCORE_DIGIT + buf[i] - '0', HUD_SCORE_VALUE_X + i * 36, HUD_SCORE_VALUE_Y);
		}
	}

	GraphicsKit::RenderSprites();
	GraphicsKit::Present();
}


