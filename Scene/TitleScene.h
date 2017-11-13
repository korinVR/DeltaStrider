#ifndef ___TitleScene_h
#define ___TitleScene_h

#include "Scene.h"

class TitleScene : public Scene {
public:
    TitleScene(int item);
    virtual void Update(SceneManager* manager);
    virtual void Render(SceneManager* manager);

    static const int ITEM_MAX = 3;
    static const int ITEM_GAME_START = 0;
    static const int ITEM_OPTION = 1;
    static const int ITEM_CREDIT = 2;
    static const int ITEM_EXIT = 3;

private:
    int alpha;
    int item;

    bool started;
    int count;

    static const int START_DEMO_TIME = 3000;
    int startDemoCount;
};

#endif // ___TitleScene_h


