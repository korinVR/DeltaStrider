#ifndef ___Base_h
#define ___Base_h

#include "GameObject.h"

class BaseShield;

class Base : public GameObject {
public:
    Base(Vector position);

    virtual void Update();
    virtual void Render();
    
    virtual void Collided();
    
private:
    shared_ptr<BaseShield> shield;
    int hp;

    float rotation;
    bool flash;

    float shotSpeed;
    int shotInterval;
    int shotCount;
};

#endif // ___Base_h


