#include "OptionScene.h"

#include "../InputKit.h"
#include "../SoundKit.h"
#include "../GraphicsKit.h"

#include "TitleScene.h"

#include "../Game.h"
#include "../ResourceId.h"

const int ITEM_MAX = 2;
const int ITEM_DIFFICULTY = 0;
const int ITEM_BGM = 1;
const int ITEM_EXIT = 2;

OptionScene::OptionScene()
{
	item = 0;
}

void OptionScene::Update(SceneManager* manager)
{
	if (InputKit::stickUpCount == 1 && item > 0) {
		item--;
		SoundKit::PlaySE(SE_CURSOR);
	}
	if (InputKit::stickDownCount == 1 && item < ITEM_MAX) {
		item++;
		SoundKit::PlaySE(SE_CURSOR);
	}

	if (InputKit::stickLeftCount == 1) {
		if (item == ITEM_DIFFICULTY) {
			if (Game::difficulty > Game::DIFFICULTY_EASY) {
				Game::difficulty--;
				SoundKit::PlaySE(SE_CURSOR);
			}
		}
		if (item == ITEM_BGM) {
			SoundKit::PlaySE(SE_CURSOR);
			Game::bgm = !Game::bgm;
		}
	}
	if (InputKit::stickRightCount == 1) {
		if (item == ITEM_DIFFICULTY) {
			if (Game::difficulty < Game::DIFFICULTY_HARD) {
				Game::difficulty++;
				SoundKit::PlaySE(SE_CURSOR);
			}
		}
		if (item == ITEM_BGM) {
			SoundKit::PlaySE(SE_CURSOR);
			Game::bgm = !Game::bgm;
		}
	}

	if (InputKit::fireButtonCount == 1) {
		switch (item) {
		case ITEM_DIFFICULTY:
			SoundKit::PlaySE(SE_CURSOR);
			if (++Game::difficulty > Game::DIFFICULTY_HARD) {
				Game::difficulty = Game::DIFFICULTY_EASY;
			}
			break;

		case ITEM_BGM:
			SoundKit::PlaySE(SE_CURSOR);
			Game::bgm = !Game::bgm;
			break;

		case ITEM_EXIT:
			SoundKit::PlaySE(SE_SELECT);
			ChangeScene(manager, new TitleScene(TitleScene::ITEM_OPTION));
			break;
		}
	}
}

void OptionScene::Render(SceneManager* manager)
{
	const float CURSOR_X = 158;
	const float CURSOR_Y = 164;
	const float CURSOR_DY = 22;
	const float SX = CURSOR_X + 140;

	GraphicsKit::Clear();
	GraphicsKit::PutSprite(SPRITE_OPTION, 0, 0);
	GraphicsKit::PutSprite(SPRITE_CURSOR, CURSOR_X, CURSOR_Y + item * CURSOR_DY);
	GraphicsKit::PutSprite(SPRITE_OPTION_TEXT_EASY + Game::difficulty, SX, CURSOR_Y - 1);
	GraphicsKit::PutSprite(SPRITE_OPTION_TEXT_OFF - Game::bgm, SX, CURSOR_Y + CURSOR_DY);
	GraphicsKit::RenderSprites();
	GraphicsKit::Present();
}


