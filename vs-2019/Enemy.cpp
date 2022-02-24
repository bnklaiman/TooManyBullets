#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

#include "Enemy.h"

Enemy::Enemy() {
	// Set up "enemy" sprite
	setSprite("enemy");

	// Set object type
	setType("Enemy");

	// Set starting location at top of window, random x-axis
	int world_horiz = (int)WM.getBoundary().getHorizontal();
	int world_vert = (int)WM.getBoundary().getVertical();
	df::Vector p(world_horiz / 2, 3);
	setPosition(p);
}