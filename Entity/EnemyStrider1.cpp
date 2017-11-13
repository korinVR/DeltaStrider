#include "EnemyStrider1.h"

#include "../Radar.h"

#include "Debris.h"
#include "EnemyShot.h"
#include "Explosion.h"
#include "MyShip.h"

const float RESISTANCE = 0.997f;

const float SHOT_SPEEDS[] = {0.02f, 0.04f, 0.06f };
const int SHOT_INTERVALS[] = { 1400, 1000, 800 };

EnemyStrider1::EnemyStrider1(Vector position)
{
	this->position = position;
	position.z = 0;

	acceleration = Random::GetReal(0.0004f, 0.0006f) + Game::rank * 0.0001f;
	sensorPositionFactor = Random::GetReal(100, 250);
	repulsiveForceFactor = Random::GetReal(0.1f, 0.3f);

	rotation = 0;
	rotationVelocity = 0;

	roll = 0;
	jetScale = 0;

	shotSpeed = SHOT_SPEEDS[Game::rank];
	shotInterval = SHOT_INTERVALS[Game::rank];
	shotCount = 0;
}

void EnemyStrider1::Update()
{
	Vector myShipPosition = Game::myShip->GetPosition();
	Vector myShipVelocity = Game::myShip->GetVelocity();
	myShipPosition.z = 0;
	myShipVelocity.z = 0;
	
	Vector targetPosition = myShipPosition + (myShipVelocity * Length(myShipPosition - position) * 1.0f);
	Vector a1 = Unit(targetPosition - position) * acceleration;

	Vector sensorPosition = position + velocity * sensorPositionFactor;
	Vector difference = targetPosition - sensorPosition;
	float distance = Length(difference);
	Vector a2 = (-Unit(difference) / Square(distance)) * repulsiveForceFactor;

	Vector accelerationVector = a1 + a2;

	velocity += accelerationVector;
	velocity *= RESISTANCE;
	position += velocity;

	float direction = atan2(accelerationVector.y, accelerationVector.x);
	float d = Norm(direction - rotation);
	rotationVelocity += d * 0.002f;

	rotationVelocity *= 0.9f;
	rotation = Norm(rotation + rotationVelocity);
	roll = rotationVelocity * 80.0f;
	
	jetScale = (Length(accelerationVector) / acceleration
		- fabs(rotationVelocity * 10) + Random::GetReal(0, 0.25f)) * 0.6f;


	shotCount++;
	if (InSight()) {
		CollisionDetector::RegisterN(CollisionBall(this, position, 1.0f));

		float distance = Length(Game::myShip->GetPosition() - position);
		if (20 < distance && distance < 50) {
			if (shotCount > shotInterval) {
				Vector shotVelocity = Unit(myShipPosition - position) * shotSpeed;
				Game::enemyShots.Append(new EnemyShot(position, shotVelocity + myShipVelocity * 0.7f));
				shotCount = 0;
			}
		}
	}
}

void EnemyStrider1::Render()
{
	if (InSight()) {
		Matrix worldMatrix = 
			TranslationMatrix(-0.5f, 0, 0) *
			RotationMatrixX(roll) * RotationMatrixZ(rotation) * 
			TranslationMatrix(position);
		
		GraphicsKit::RenderMesh(MESH_ENEMY_STRIDER_1, worldMatrix);
		GraphicsKit::PutEffect(SPRITE_JET, position, jetScale, rotation);
	}

	Radar::RenderEnemyBlip(position);
}

void EnemyStrider1::Collided()
{
	SoundKit::PlaySE(SE_EXPLOSION_SMALL);
	CreateSmallExplosionSuite(position, velocity);
	for (int i = 0; i < 5; i++) {
		CreateDebris(MESH_METAL_DEBRIS_1, position, velocity);
		CreateDebris(MESH_METAL_DEBRIS_2, position, velocity);
	}
	if (Game::timeRemaining > 0) {
		Game::score += 70;
	}
	Die();
}


