#ifndef ___StageCamera_h
#define ___StageCamera_h

#include "Camera.h"

class StageCamera : public Camera {
public:
    StageCamera();

    virtual void Update();
    virtual void Render();
    
private:
    Vector offset;
    float fov;
};

#endif // ___StageCamera_h


