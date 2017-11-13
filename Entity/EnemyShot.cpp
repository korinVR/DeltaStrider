#include "EnemyShot.h"

#include "MyShip.h"

EnemyShot::EnemyShot(Vector position, Vector velocity)
{
    this->position = position;
    this->velocity = velocity;
}

void EnemyShot::Update()
{
    rotation += Radian(5);
    position += velocity;
    
    if (Game::myShip->GetState() == MyShip::STATE_NORMAL) {
        if (Length(Game::myShip->GetPosition() - position) < 1.0f) {
            Game::myShip->Collided();
            Die();
            return;
        }
    }

    if (!InSight()) {
        Die();
    }
}

void EnemyShot::Render()
{
    Vector spritePosition = GraphicsKit::GetScreenPosition(position);

    Vector v = GraphicsKit::GetScreenPosition(position + Vector(0.06f, 0, 0));
    float scale = v.x - spritePosition.x;

    GraphicsKit::PutSprite(SPRITE_ENEMY_SHOT, spritePosition.x, spritePosition.y, scale, rotation);
}


