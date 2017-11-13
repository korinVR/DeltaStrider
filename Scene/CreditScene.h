#ifndef ___CreditScene_h
#define ___CreditScene_h

#include "Scene.h"

class CreditScene : public Scene {
public:
	CreditScene();
	virtual void Update(SceneManager* manager);
	virtual void Render(SceneManager* manager);

private:
};

#endif // ___CreditScene_h


