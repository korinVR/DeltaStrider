#ifndef ___StageText_h
#define ___StageText_h

#include "GameObject.h"

class StageText : public GameObject {
public:
	StageText();

	virtual void Update();
	virtual void Render();
	
private:
	int count;
	int alpha;
	float scale;
};

#endif // ___StageText_h


