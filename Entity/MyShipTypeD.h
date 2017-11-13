#ifndef ___MyShipTypeD_h
#define ___MyShipTypeD_h

#include "MyShip.h"

class MyShipTypeD : public MyShip {
public:
	MyShipTypeD();

	virtual void Update();
	virtual void Render();

	virtual void SetState(int state);

	virtual void Collided();
	
private:
	float rotationVelocity;

	float roll;
	int rapidFireCount;

	float jetScale;
	bool showMuzzleFlash;
};

#endif // ___MyShipTypeD_h


