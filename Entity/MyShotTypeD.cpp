#include "MyShotTypeD.h"

#include "Explosion.h"
#include "MyShip.h"

MyShotTypeD::MyShotTypeD(Vector position, Vector velocity, float rotation)
{
    this->position = position;
    this->velocity = velocity;
    this->rotation = rotation;
}

void MyShotTypeD::Update()
{
    position += velocity;
    
    if (!InSight()) {
        Die();
    }

    CollisionDetector::RegisterM(CollisionBall(this, position, 0.5f));
}

void MyShotTypeD::Render()
{
    GraphicsKit::PutEffect(SPRITE_MYSHOT_TYPED, position, 1, rotation);
}

void MyShotTypeD::Collided()
{
    Game::effects.Append(new Explosion(position, Vector(0, 0, 0), 0.5f, 0.5f));
    Die();
}

