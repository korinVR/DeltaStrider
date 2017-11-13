#ifndef ___OptionScene_h
#define ___OptionScene_h

#include "Scene.h"

class OptionScene : public Scene {
public:
    OptionScene();
    virtual void Update(SceneManager* manager);
    virtual void Render(SceneManager* manager);

private:
    int item;
};

#endif // ___OptionScene_h


