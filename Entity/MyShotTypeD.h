#ifndef ___MyShotTypeD_h
#define ___MyShotTypeD_h

#include "GameObject.h"

class MyShotTypeD : public GameObject {
public:
    MyShotTypeD(Vector position, Vector velocity, float rotation);

    virtual void Update();
    virtual void Render();

    virtual void Collided();

private:
    float rotation;
};

#endif // ___MyShotTypeD_h


