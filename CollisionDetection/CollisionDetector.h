#ifndef ___CollisionDetector_h
#define ___CollisionDetector_h

#include <vector>
using namespace std;

#include "CollisionBall.h"

class CollisionDetector {
public:
	static void Clear();
	static void RegisterM(const CollisionBall& ball);
	static void RegisterN(const CollisionBall& ball);
	static void Detect();

private:
	static vector<CollisionBall> m;
	static vector<CollisionBall> n;
};

#endif // ___CollisionDetector_h


