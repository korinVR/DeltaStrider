#ifndef ___GameOverScene_h
#define ___GameOverScene_h

#include "Scene.h"

class GameOverScene : public Scene {
public:
    GameOverScene();
    virtual void Update(SceneManager* manager);
    virtual void Render(SceneManager* manager);

private:
    int count;
};

#endif // ___GameOverScene_h


