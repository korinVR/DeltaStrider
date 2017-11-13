#include "GameObject.h"

#include "MyShip.h"

GameObject::GameObject() : state(0), count(0)
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
    count++;
}

void GameObject::Render()
{
}

void GameObject::SetState(int state)
{
    this->state = state;
    count = 0;
}

void GameObject::Collided()
{
}

bool GameObject::InSight() const
{
    const float IN_SIGHT_DISTANCE = 80;

    if (LengthSquared(Game::myShip->GetPosition() - position) < Square(IN_SIGHT_DISTANCE)) {
        return true;
    }
    return false;
}
