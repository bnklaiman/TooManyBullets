#include <WorldManager.h>

#include "Boss.h"
#include "EventView.h"
#include "EventStep.h"
#include "utility.h"

#include <random>

Boss::Boss() {
	setSprite("boss");
	setType("Boss");

	int world_horiz = (int)WM.getBoundary().getHorizontal();
	int world_vert = (int)WM.getBoundary().getVertical();
	df::Vector p(world_horiz / 2, world_vert / 4);
	bossHealth = 1000;

	setPosition(p);

	stepsSinceLastAttack = 0;
	attackThreshold = 30;

	stepsSinceMove = 60;
	moveStepsRemaining = 0;
	moveThreshold = 90;

	registerInterest(df::STEP_EVENT);
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

int Boss::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}
	return 0;
}

void Boss::step() {
	//do movement stuff
	// if the steps left to move is above 0, let it keep moving (if it will move in bounds) and decrement
	if (moveStepsRemaining > 0) {
		// if predicted position is outside world boundary
		if (!df::boxContainsPosition(WM.getBoundary(), predictPosition())) {
			moveStepsRemaining = 0;
			stepsSinceMove = 0;
			setSpeed(0);
		}
		else {
			// else it will be in the world, keep moving as normal
			moveStepsRemaining--;
		}

	}
	else if (moveStepsRemaining <= 0) {
		// else, we should stop moving and begin incrementing steps since move
		setSpeed(0);
		stepsSinceMove++;
		// if steps since move is above the threshold, roll to move
		if (stepsSinceMove > moveThreshold) {
			tryToMove();
		}
	}
}

void Boss::tryToMove() {
	float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (chance < 0.01) {
		// 1% chance to move each tick after cooldown
		stepsSinceMove = 0;
		// random number of steps to move
		moveStepsRemaining = 45 + (rand() % static_cast<int>(90 - 25 + 1));
		// random direction to move
		float x = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
		float y = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
		setDirection(df::Vector(x, y));
		setSpeed(0.15);
	}
}