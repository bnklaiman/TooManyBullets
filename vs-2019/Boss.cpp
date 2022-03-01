#include <WorldManager.h>

#include "Boss.h"

Boss::Boss() {
	setSprite("boss");
	setType("Boss");

	int world_horiz = (int)WM.getBoundary().getHorizontal();
	int world_vert = (int)WM.getBoundary().getVertical();
	df::Vector p(world_horiz / 4, world_vert / 2);
	setPosition(p);
}