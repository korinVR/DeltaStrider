#ifndef ___EntityList_h
#define ___EntityList_h

#include <list>
using namespace std;

#include <boost/shared_ptr.hpp>
using namespace boost;

class Entity;

typedef shared_ptr<Entity> EntityPtr;

class EntityList {
public:
	EntityList();
	~EntityList();

	void Clear();
	void Append(Entity* entity);
	void Insert(Entity* entity);
	
	void Update();
	void Render();
	int GetSize() const;
	
private:
	list<EntityPtr> l;
};

#endif // ___EntityList_h


