#include "Radar.h"

#include "GraphicsKit.h"

#include "Game.h"
#include "ResourceId.h"

namespace {
	const float RADAR_X = Game::SCREEN_W - 180 - 2;
	const float RADAR_Y = Game::SCREEN_H - 180 - 2;
	
	const float RADAR_X0 = RADAR_X + 17;
	const float RADAR_Y0 = RADAR_Y + 18;
	const float RADAR_X1 = RADAR_X + 154;
	const float RADAR_Y1 = RADAR_Y + 155;
}

void Radar::RenderRadar()
{
	GraphicsKit::PutSprite(SPRITE_HUD_RADAR, RADAR_X, RADAR_Y);
}

void Radar::RenderMyShipBlip(Vector position, float rotation)
{
	GraphicsKit::PutSprite(SPRITE_HUD_RADAR_BLIP_MYSHIP, 
		(RADAR_X0 + RADAR_X1) / 2 + (RADAR_X1 - RADAR_X0) * (position.x / (Game::FIELD_SIZE * 2)), 
		(RADAR_Y0 + RADAR_Y1) / 2 - (RADAR_Y1 - RADAR_Y0) * (position.y / (Game::FIELD_SIZE * 2)), 
		1, Radian(90) - rotation);
}

void Radar::RenderEnemyBlip(Vector position)
{
	GraphicsKit::PutSprite(SPRITE_HUD_RADAR_BLIP_ENEMY, 
		(RADAR_X0 + RADAR_X1) / 2 + (RADAR_X1 - RADAR_X0) * (position.x / (Game::FIELD_SIZE * 2)), 
		(RADAR_Y0 + RADAR_Y1) / 2 - (RADAR_Y1 - RADAR_Y0) * (position.y / (Game::FIELD_SIZE * 2)));
}

void Radar::RenderTargetEnemyBlip(Vector position)
{
	GraphicsKit::PutSprite(SPRITE_HUD_RADAR_BLIP_TARGET_ENEMY, 
		(RADAR_X0 + RADAR_X1) / 2 + (RADAR_X1 - RADAR_X0) * (position.x / (Game::FIELD_SIZE * 2)), 
		(RADAR_Y0 + RADAR_Y1) / 2 - (RADAR_Y1 - RADAR_Y0) * (position.y / (Game::FIELD_SIZE * 2)));
}


