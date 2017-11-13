#include "Asteroid.h"

#include "../Radar.h"

#include "Debris.h"
#include "Explosion.h"
#include "MyShip.h"

Asteroid::Asteroid()
{
	position.x = Random::GetReal(-Game::OVERFIELD_SIZE, Game::OVERFIELD_SIZE);
	position.y = Random::GetReal(-Game::OVERFIELD_SIZE, Game::OVERFIELD_SIZE);
	position.z = 0;

	velocity.x = Random::GetNormalDistributionReal(0, 0.02f);
	velocity.y = Random::GetNormalDistributionReal(0, 0.02f);
	velocity.z = 0;
}

Asteroid::Asteroid(Vector position, Vector velocity)
{
	this->position = position;
	this->velocity = velocity;
}

void Asteroid::Update()
{
	position += velocity;

	if (position.x >  Game::OVERFIELD_SIZE) { position.x -= Game::OVERFIELD_SIZE * 2; }
	if (position.x < -Game::OVERFIELD_SIZE) { position.x += Game::OVERFIELD_SIZE * 2; }
	if (position.y >  Game::OVERFIELD_SIZE) { position.y -= Game::OVERFIELD_SIZE * 2; }
	if (position.y < -Game::OVERFIELD_SIZE) { position.y += Game::OVERFIELD_SIZE * 2; }

	if (InSight()) {
		CollisionDetector::RegisterN(CollisionBall(this, position, 5.0f));
	}
}

void Asteroid::Render()
{
	if (InSight()) {
		GraphicsKit::RenderMesh(MESH_ASTEROID, ScalingMatrix(5) * TranslationMatrix(position));
	}
}

void Asteroid::Collided()
{
	CreateExplosionSuite(position, velocity);
	Game::score += 200;
	Die();

	for (int i = 0; i < 30; i++) {
		CreateDebris(MESH_ASTEROID_DEBRIS, position, velocity);
	}
}


