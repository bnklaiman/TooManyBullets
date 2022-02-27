#include <LogManager.h>
#include <ResourceManager.h>
#include <WorldManager.h>
#include <EventStep.h>
#include <utility.h>

#include "Enemy.h"
#include "Explosion.h"
#include "Bullet.h"

#include <random>


Enemy::Enemy() {
	// Set up "enemy" sprite
	setSprite("enemy");

	// Set object type
	setType("Enemy");

	setSolidness(df::HARD);

	// Set starting location at top of window, random x-axis
	int world_horiz = (int)WM.getBoundary().getHorizontal();
	int world_vert = (int)WM.getBoundary().getVertical();
	df::Vector p(world_horiz / 2, 3);
	setPosition(p);

	stepsSinceLastAttack = 0;
	attackThreshold = 30;

	stepsSinceMove = 60;
	moveStepsRemaining = 0;
	moveThreshold = 45;

	registerInterest(df::STEP_EVENT);
}

int Enemy::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}
	return 0;
}

void Enemy::step() {
	stepsSinceLastAttack++;
	// if its been longer than our attack threshold, we have a chance to fire.
	if (stepsSinceLastAttack > attackThreshold) {
		float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		if (chance > 0.97) {
			basicAttack();
			stepsSinceLastAttack = 0;
		}
	}
	
	//do movement stuff
	// if the steps left to move is above 0, let it keep moving (if it will move in bounds) and decrement
	if (moveStepsRemaining > 0) {
		// if predicted position is outside world boundary
		if (!df::boxContainsPosition(WM.getBoundary(), predictPosition())) {
			moveStepsRemaining = 0;
			stepsSinceMove = 0;
			setSpeed(0);
		} else {
			// else it will be in the world, keep moving as normal
			moveStepsRemaining--;
		}
		
	} else if (moveStepsRemaining <= 0) {
		// else, we should stop moving and begin incrementing steps since move
		setSpeed(0);
		stepsSinceMove++;
		// if steps since move is above the threshold, roll to move
		if (stepsSinceMove > moveThreshold) {
			tryToMove();
		}
	}
}

void Enemy::tryToMove() {
	float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (chance < 0.01) {
		// 1% chance to move each tick after cooldown
		stepsSinceMove = 0;
		// random number of steps to move
		moveStepsRemaining = 15 + (rand() % static_cast<int>(60 - 15 + 1));
		// random direction to move
		float x = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
		float y = -1 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (1 - -1)));
		setDirection(df::Vector(x, y));
		setSpeed(0.1);
	}
}

void Enemy::basicAttack() {
	Bullet* b = new Bullet(getPosition(), true);
	b->shooter = getType();
	b->setVelocity(df::Vector(0, 3));
}