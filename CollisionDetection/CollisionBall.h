#ifndef ___CollisionBall_h
#define ___CollisionBall_h

#include "../Sparkler/Sparkler.h"
using namespace Sparkler;

class GameObject;

class CollisionBall {
public:
	CollisionBall(GameObject* object, Vector position, float radius) : 
		object(object), position(position), radius(radius) {}

	GameObject* object;
	Vector position;
	float radius;
};

#endif // ___CollisionBall_h


