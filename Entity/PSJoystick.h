#ifndef ___PSJoystick_h
#define ___PSJoystick_h

#include "Entity.h"

class PSJoystick : public Entity {
public:
    PSJoystick();

    virtual void Update();
    virtual void Render();
};

#endif // ___PSJoystick_h


