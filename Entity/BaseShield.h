#ifndef ___BaseShield_h
#define ___BaseShield_h

#include "GameObject.h"

class Base;

class BaseShield : public GameObject {
public:
	BaseShield(Vector position);

	virtual void Update();
	virtual void Render();
	
	virtual void Collided();
	
private:
	float rotation;
	bool flash;
};

#endif // ___BaseShield_h


