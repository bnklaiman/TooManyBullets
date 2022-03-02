#include <WorldManager.h>

#include "Boss.h"
#include "EventView.h"

Boss::Boss() {
	setSprite("boss");
	setType("Boss");

	int world_horiz = (int)WM.getBoundary().getHorizontal();
	int world_vert = (int)WM.getBoundary().getVertical();
	df::Vector p(world_horiz / 2, world_vert / 4);
	bossHealth = 1000;

	setPosition(p);
}

Boss::Boss(int startingHealth) : Boss() {
	bossHealth = startingHealth;
}

int Boss::getBossHealth() {
	return bossHealth < 0? 0 : bossHealth;
}

void Boss::setBossHealth(int health) {
	bossHealth = health;
	if (bossHealth < 0) bossHealth = 0;
	df::EventView ev("Boss:", bossHealth, false);
	WM.onEvent(&ev);
}