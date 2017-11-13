#include "Game.h"

#include "Entity/MyShip.h"

const float Game::FIELD_SIZE = 200;
const float Game::OVERFIELD_SIZE = 280;

int Game::stageTime[STAGE_MAX + 1];
int Game::difficulty;
int Game::rank;

bool Game::bgm;
bool Game::exit;

int Game::demo = 0;

MyShipPtr Game::myShip;
CameraPtr Game::camera;

EntityList Game::myShots;
EntityList Game::effects;
EntityList Game::enemies;
EntityList Game::targetEnemies;
EntityList Game::enemyShots;

int Game::stage;
int Game::left;
int Game::score;
int Game::scoreCount;
int Game::timeRemaining;


