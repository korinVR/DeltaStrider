#ifndef ___EnemyStrider1_h
#define ___EnemyStrider1_h

#include "GameObject.h"

class EnemyStrider1 : public GameObject {
public:
	EnemyStrider1(Vector position);

	virtual void Update();
	virtual void Render();
	
	virtual void Collided();
	
private:
	float acceleration;
	float sensorPositionFactor;
	float repulsiveForceFactor;

	float rotation;
	float rotationVelocity;

	float roll;
	float jetScale;

	float shotSpeed;
	int shotInterval;
	int shotCount;
};

#endif // ___EnemyStrider1_h


