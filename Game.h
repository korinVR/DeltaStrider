#ifndef ___Game_h
#define ___Game_h

#include <boost/smart_ptr.hpp>
using namespace boost;

#include "Entity/EntityList.h"
class Camera;
class MyShip;

typedef shared_ptr<Camera> CameraPtr;
typedef shared_ptr<MyShip> MyShipPtr;

class Game {
public:
	static const int SCREEN_W = 640;
	static const int SCREEN_H = 480;

	static const int INTERNAL_FPS = 300;

	static const float FIELD_SIZE;
	static const float OVERFIELD_SIZE;
	
	static const int STAGE_MAX = 15;
	static int stageTime[STAGE_MAX + 1];

	static const int DIFFICULTY_EASY = 0;
	static const int DIFFICULTY_NORMAL = 1;
	static const int DIFFICULTY_HARD = 2;
	static int difficulty;
	static int rank;

	static bool bgm;
	static bool exit;

	static const int DEMO_NUM = 3;
	static int demo;

	static MyShipPtr myShip;
	static CameraPtr camera;
	
	static EntityList myShots;
	static EntityList effects;
	static EntityList enemies;
	static EntityList targetEnemies;
	static EntityList enemyShots;

	static int stage;
	static int left;
	static int score;
	static int scoreCount;
	static int timeRemaining;
};

#endif // ___Game_h


