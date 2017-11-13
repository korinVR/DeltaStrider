#ifndef ___Scene_h
#define ___Scene_h

#include "SceneManager.h"

class Scene {
public:
	Scene();
	virtual ~Scene();
	virtual void Update(SceneManager* manager) = 0;
	virtual void Render(SceneManager* manager) = 0;
	
protected:
	void ChangeScene(SceneManager* manager, Scene* scene);
};

#endif // ___Scene_h


