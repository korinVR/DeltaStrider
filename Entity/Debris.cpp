#include "Debris.h"

#include "MyShip.h"

#include "../Game.h"

Debris::Debris(int meshNo, Vector position, Vector velocity, float scale) : 
	meshNo(meshNo), scale(scale)
{
	this->position = position;
	this->velocity = velocity;

	rotationMatrix = IdentityMatrix();
	rotationVelocityMatrix = 
		RotationMatrixX(Radian(Random::GetNormalDistributionReal(0.0f, 1.0f))) * 
		RotationMatrixY(Radian(Random::GetNormalDistributionReal(0.0f, 1.0f))) * 
		RotationMatrixZ(Radian(Random::GetNormalDistributionReal(0.0f, 1.0f)));
}

void Debris::Update()
{
	velocity *= 0.999f;
	rotationMatrix *= rotationVelocityMatrix;
	
	position += velocity;
	
	if (!InSight()) {
		Die();
	}
}

void Debris::Render()
{
	GraphicsKit::RenderMesh(meshNo, 
		ScalingMatrix(scale) * rotationMatrix * TranslationMatrix(position));
}

void CreateDebris(int meshNo, Vector position, Vector velocity)
{
	Vector offsetVelocity(
		Random::GetNormalDistributionReal(0, 0.08f),
		Random::GetNormalDistributionReal(0, 0.08f),
		Random::GetNormalDistributionReal(0, 0.08f));

	Game::enemies.Append(new Debris(
		meshNo, position, velocity + offsetVelocity, Random::GetReal(0.1f, 0.8f)));
}


