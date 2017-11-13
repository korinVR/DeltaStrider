#include "Entity.h"

Entity::Entity() : alive(true)
{
}

Entity::~Entity()
{
}

void Entity::Update()
{
}

void Entity::Render()
{
}

void Entity::Die()
{
    alive = false;
}


