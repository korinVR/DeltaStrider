#ifndef ___Radar_h
#define ___Radar_h

#include "Sparkler/Sparkler.h"
using namespace Sparkler;

class Radar {
public:
	static void RenderMyShipBlip(Vector position, float rotation);
	static void RenderEnemyBlip(Vector position);
	static void RenderTargetEnemyBlip(Vector position);
	static void RenderRadar();
};

#endif // ___Radar_h


