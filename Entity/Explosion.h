#ifndef ___Explosion_h
#define ___Explosion_h

#include "GameObject.h"

class Explosion : public GameObject {
public:
	Explosion(Vector position, Vector velocity, float scale, float frmaeSpeed);

	virtual void Update();
	virtual void Render();

private:
	float scale;
	float rotation;
	
	float frameSpeed;
	float frame;
};

void CreateSmallExplosionSuite(Vector position, Vector velocity);
void CreateExplosionSuite(Vector position, Vector velocity);
void CreateBigExplosionSuite(Vector position, Vector velocity);

#endif // ___Explosion_h


