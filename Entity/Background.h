#ifndef ___Background_h
#define ___Background_h

#include "GameObject.h"

class Background : public GameObject {
public:
    Background(int no);

    virtual void Update();
    virtual void Render();

private:
    int no;
};

#endif // ___Background_h


