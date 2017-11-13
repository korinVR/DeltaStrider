#ifndef ___EnemyShot_h
#define ___EnemyShot_h

#include "GameObject.h"

class EnemyShot : public GameObject {
public:
	EnemyShot(Vector position, Vector velocity);

	virtual void Update();
	virtual void Render();
	
private:
	float rotation;
};

#endif // ___EnemyShot_h


