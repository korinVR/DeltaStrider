#ifndef ___Asteroid_h
#define ___Asteroid_h

#include "GameObject.h"

class Asteroid : public GameObject {
public:
	Asteroid();
	Asteroid(Vector position, Vector velocity);

	virtual void Update();
	virtual void Render();

	virtual void Collided();
};

#endif // ___Asteroid_h


