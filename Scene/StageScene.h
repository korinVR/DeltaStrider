#ifndef ___StageScene_h
#define ___StageScene_h

#include "Scene.h"

#include "../Entity/EntityList.h"

class StageScene : public Scene {
public:
    StageScene();
    virtual void Update(SceneManager* manager);
    virtual void Render(SceneManager* manager);

    void GotoNextStage();

private:
    EntityList huds;
    EntityList backgrounds;

    float leftShipAngle;

    bool stageCleared;
    bool gotoNextStage;

    int st;
};

#endif // ___StageScene_h


