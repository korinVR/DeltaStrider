#include "Entity.h"
#include "EntityList.h"

EntityList::EntityList()
{
}

EntityList::~EntityList()
{
	Clear();
}

void EntityList::Clear()
{
	l.clear();
}

void EntityList::Append(Entity* entity)
{
	l.push_back(static_cast<EntityPtr>(entity));
}

void EntityList::Insert(Entity* entity)
{
	l.push_front(static_cast<EntityPtr>(entity));
}

void EntityList::Update()
{
	for (list<EntityPtr>::iterator it = l.begin(); it != l.end(); ) {
		if ((*it)->IsAlive()) {
			(*it)->Update();
			++it;
		} else {
			it = l.erase(it);
		}
	}
}

void EntityList::Render()
{
	for (list<EntityPtr>::iterator it = l.begin(); it != l.end(); ) {
		if ((*it)->IsAlive()) {
			(*it)->Render();
			++it;
		} else {
			it = l.erase(it);
		}
	}

	//for_each(l.begin(), l.end(), mem_fn(&Entity::render));
}

int EntityList::GetSize() const
{
	return static_cast<int>(l.size());
}


