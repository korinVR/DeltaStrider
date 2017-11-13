#include "CollisionDetector.h"

#include "../Entity/GameObject.h"

vector<CollisionBall> CollisionDetector::m;
vector<CollisionBall> CollisionDetector::n;

void CollisionDetector::Clear()
{
	m.clear();
	n.clear();
}

void CollisionDetector::RegisterM(const CollisionBall& ball)
{
	m.push_back(ball);
}

void CollisionDetector::RegisterN(const CollisionBall& ball)
{
	n.push_back(ball);
}

void CollisionDetector::Detect()
{
	for (vector<CollisionBall>::iterator im = m.begin(); im != m.end(); ++im) {
		for (vector<CollisionBall>::iterator in = n.begin(); in != n.end(); ++in) {
			if (im->object->IsAlive() && in->object->IsAlive()) {
				float distanceSquared = LengthSquared(im->position - in->position);
				
				if (distanceSquared <= Square(im->radius + in->radius)) {
					im->object->Collided();
					in->object->Collided();
				}
			}
		}
	}
}


