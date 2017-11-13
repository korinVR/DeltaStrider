#ifndef ___Camera_h
#define ___Camera_h

#include "GameObject.h"

class Camera : public GameObject {
public:
    Camera();

    virtual void Update();
    virtual void Render();
};

#endif // ___Camera_h


