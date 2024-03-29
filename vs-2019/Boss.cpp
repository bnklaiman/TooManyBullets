#include <WorldManager.h>
#include "ObjectList.h"
#include "ObjectListIterator.h"

#include "Boss.h"
#include "EventView.h"
#include "EventStep.h"
#include "utility.h"
#include "Bullet.h"
#include "Player.h"

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
	attackThreshold = 175;

	stepsSinceMove = 60;
	moveStepsRemaining = 0;
	moveThreshold = 90;

	// config for burst stars
	starAttackThreshold = 150;
	stepsSinceStarAttack = 0;
	starAttackBurstFiredCount = 0;
	starAttackBurstCount = 3;
	starAttackBurstThreshold = 45;
	stepsSinceStarAttackBurstFired = 0;
	inStarAttack = false;

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

	// star (large projectile) attack
	// if out of cooldown roll
	stepsSinceStarAttack++;
	if (stepsSinceStarAttack > starAttackThreshold) {
		tryStartingStarAttack();
	}

	if (inStarAttack) {
		tryStarBurst();
	}

	stepsSinceLastAttack++;
	if (stepsSinceLastAttack > attackThreshold) {
		int n = 1 + (rand() % static_cast<int>(3 - 1 + 1));
		circleAttack(n, 25);
		stepsSinceLastAttack = 0;
	}
}

void Boss::tryStartingStarAttack() {
	// roll for star attack
	float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (chance < 0.1) {
		inStarAttack = true;
		stepsSinceStarAttack = 0;
		starAttackBurstFiredCount = 0;
		stepsSinceStarAttackBurstFired = 0;
	}
}

void Boss::tryStarBurst() {
	// in the attack, check if cooldown to fire another expired
	stepsSinceStarAttackBurstFired++;
	if (stepsSinceStarAttackBurstFired > starAttackBurstThreshold) {
		stepsSinceStarAttackBurstFired = 0;
		// cooldown good, now see if we need to fire another bullet
		if (starAttackBurstFiredCount < starAttackBurstCount) {
			// need to fire a bullet
			starAttackBurstFiredCount++;
			Bullet* b = new Bullet(getPosition(), true);
			b->setSprite("Star");
			// get position of player
			df::ObjectList ol = WM.objectsOfType("Player");
			df::ObjectListIterator* li = new df::ObjectListIterator(&ol);
			Player* player = dynamic_cast<Player*> (li->currentObject());
			if (!player || !this) return;
			df::Vector playerPos = player->getPosition();
			df::Vector fireDirection = playerPos - getPosition();
			b->setDirection(fireDirection);
			b->setSpeed(0.01);
			b->shooter = "Enemy";
			b->setAltitude(1);

			if (starAttackBurstFiredCount == starAttackBurstCount) {
				// if we've now fired the full burst, exit the attack.
				inStarAttack = false;
				stepsSinceStarAttack = 0;
				starAttackBurstFiredCount = 0;
				stepsSinceStarAttackBurstFired = 0;
			}
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

void Boss::circleAttack(int numCircles, int numBullets) {
	df::Vector center = getPosition();
	float radius = 5 + (rand() % static_cast<int>(7 - 5 + 1));
	float dTheta = 360.0f / numBullets;
	// initial position of bullets (bottom of circle)
	
	df::Vector initPos = df::Vector(center.getX(), center.getY() - radius);
	Bullet* b;
	for (int j = 0; j < numCircles; j++) {
		for (int i = 0; i < numBullets; i++) {
			// rotate by deltadeg * i
			float theta = dTheta * i;
			df::Vector pos = rotatePos(initPos, center, theta);
			// find normal from the circle... 
			df::Vector normal = df::Vector(pos.getX() - center.getX(), pos.getY() - center.getY());
			// assign attributes to bullet
			b = new Bullet(pos, true);
			b->setSprite("SimpleBullet");
			b->shooter = "Enemy";
			b->setDirection(normal);
			b->setSpeed(0.05);
		}
		radius += 1;
		initPos = df::Vector(center.getX(), center.getY() - radius);
		initPos = rotatePos(initPos, center, 25 * j);
	}
	
}

df::Vector Boss::rotatePos(df::Vector point, df::Vector center, float theta) {
	float x = point.getX() - center.getX();
	float y = point.getY() - center.getY();

	float xPrime = x * cos(theta) - y * sin(theta);
	float yPrime = x * sin(theta) + y * cos(theta);

	// create new point with new values
	df::Vector pos = df::Vector(xPrime + center.getX(), yPrime + center.getY());
	return pos;
}