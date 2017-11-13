#ifndef ___GameObject_h
#define ___GameObject_h

#include "../Sparkler/Sparkler.h"
using namespace Sparkler;

#include "../InputKit.h"
#include "../SoundKit.h"
#include "../GraphicsKit.h"

#include "../CollisionDetection/CollisionDetector.h"

#include "../Game.h"
#include "../ResourceId.h"

#include "Entity.h"

class GameObject : public Entity {
public:
	GameObject();
	virtual ~GameObject();

	virtual void SetState(int state);
	int GetState() const { return state; }
	
	virtual void Update();
	virtual void Render();
	
	Vector GetPosition() const { return position; }
	Vector GetVelocity() const { return velocity; }
	
	virtual void Collided();

	bool InSight() const;
	
protected:
	Vector position;
	Vector velocity;
	
	int state;
	int count;
};

#endif // ___GameObject_h


