#ifndef ___Debris_h
#define ___Debris_h

#include "GameObject.h"

class Debris : public GameObject {
public:
    Debris(int meshNo, Vector position, Vector velocity, float scale);

    virtual void Update();
    virtual void Render();
    
private:
    int meshNo;
    float scale;
    
    Matrix rotationMatrix;
    Matrix rotationVelocityMatrix;
};

void CreateDebris(int meshNo, Vector position, Vector velocity);

#endif // ___Debris_h


