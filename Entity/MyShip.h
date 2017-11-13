#ifndef ___MyShip_h
#define ___MyShip_h

#include "GameObject.h"

class MyShip : public GameObject {
public:
    MyShip();
    virtual ~MyShip();

    virtual void Update();
    virtual void Render();

    float GetRotation() const { return rotation; }

    static const int STATE_ADVENT = 0;
    static const int STATE_NORMAL = 1;
    static const int STATE_DEAD = 2;
    static const int STATE_STAGE_CLEAR = 3;

protected:
    float rotation;
};

float GetMyShipDistanceSquared();

#endif // ___MyShip_h


