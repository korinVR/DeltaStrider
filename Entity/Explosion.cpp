#include "Explosion.h"

#include "Flash.h"

Explosion::Explosion(Vector position, Vector velocity, float scale, float frameSpeed)
{
	this->position = position;
	this->velocity = velocity;
	this->scale = scale;
	this->frameSpeed = frameSpeed;
	
	frame = 0;
	rotation = Random::GetReal(0, PI2);
}

void Explosion::Update()
{
	velocity *= 0.99f;
	position += velocity;
	
	frame += frameSpeed;
	if (frame >= 24.0f) {
		Die();
	}
}

void Explosion::Render()
{
	GraphicsKit::PutEffect(SPRITE_EXPLOSION + static_cast<int>(frame), position, scale, rotation);
}

void CreateSmallExplosionSuite(Vector position, Vector velocity)
{
	const float SPREAD = 0.02f;
	
	for (int i = 0; i < 8; i++) {
		GameObject* object = new Explosion( 
			position, 
			Vector(velocity.x + Random::GetNormalDistributionReal(0, SPREAD), 
				   velocity.y + Random::GetNormalDistributionReal(0, SPREAD), 
				   velocity.z + Random::GetNormalDistributionReal(0, SPREAD)), 
			Random::GetNormalDistributionReal(1.0f, 0.4f), 
			Random::GetReal(0.04f, 0.1f));
		Game::effects.Append(object);
	}
	SoundKit::PlaySE(SE_EXPLOSION_SMALL);
}

void CreateExplosionSuite(Vector position, Vector velocity)
{
	const float SPREAD = 0.05f;
	
	for (int i = 0; i < 24; i++) {
		GameObject* object = new Explosion( 
			position, 
			Vector(velocity.x + Random::GetNormalDistributionReal(0, SPREAD), 
				   velocity.y + Random::GetNormalDistributionReal(0, SPREAD), 
				   velocity.z + Random::GetNormalDistributionReal(0, SPREAD)), 
			Random::GetNormalDistributionReal(1.6f, 0.6f), 
			Random::GetReal(0.04f, 0.1f));
		Game::effects.Append(object);
	}
	SoundKit::PlaySE(SE_EXPLOSION);
}

void CreateBigExplosionSuite(Vector position, Vector velocity)
{
	const float SPREAD = 0.1f;
	
	for (int i = 0; i < 32; i++) {
		GameObject* object = new Explosion( 
			position, 
			Vector(velocity.x + Random::GetNormalDistributionReal(0, SPREAD), 
				   velocity.y + Random::GetNormalDistributionReal(0, SPREAD), 
				   velocity.z + Random::GetNormalDistributionReal(0, SPREAD)), 
			Random::GetNormalDistributionReal(3.0f, 0.6f), 
			Random::GetReal(0.04f, 0.1f));
		Game::effects.Append(object);
	}

	Game::effects.Append(new Flash(position));

	SoundKit::PlaySE(SE_EXPLOSION_BIG);
}


