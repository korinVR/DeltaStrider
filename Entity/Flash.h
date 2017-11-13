#ifndef ___Flash_h
#define ___Flash_h

#include "GameObject.h"

class Flash : public GameObject {
public:
	Flash(Vector position);

	virtual void Update();
	virtual void Render();
	
private:
	float scale;
	float alpha;
};

#endif // ___Flash_h


