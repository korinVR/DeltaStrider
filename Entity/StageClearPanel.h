#ifndef ___StageClearPanel_h
#define ___StageClearPanel_h

#include "GameObject.h"

class StageScene;

class StageClearPanel : public GameObject {
public:
    StageClearPanel(StageScene* scene);

    virtual void Update();
    virtual void Render();
    
private:
    static const int STATE_SHOW_STAGE_CLEAR = 0;
    static const int STATE_INCREASE_TIME_BONUS = 1;
    static const int STATE_STOP_TIME_BONUS = 2;
    static const int STATE_ADD_SCORE = 3;
    static const int STATE_WAIT = 4;
    
    int clearTime;
    int timeBonus;
    StageScene* scene;
};

#endif // ___StageClearPanel_h


