#include <WorldManager.h>

#include "Boss.h"

Boss::Boss() {
	setSprite("boss");
	setType("Boss");

	int world_horiz = (int)WM.getBoundary().getHorizontal();
	int world_vert = (int)WM.getBoundary().getVertical();
	df::Vector p(world_horiz / 2, world_vert / 4);

	setPosition(p);
}

int Boss::getBossHealth() {
	return bossHealth;
}

void Boss::setBossHealth(int health) {
	bossHealth = health;
}