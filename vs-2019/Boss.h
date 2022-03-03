#pragma once

#include <EventCollision.h>
#include <Object.h>

class Boss : public df::Object {
private:
	int bossHealth;
	int stepsSinceLastAttack;
	int attackThreshold;

	int stepsSinceMove;
	int moveStepsRemaining;
	int moveThreshold;

	int starAttackThreshold;
	int stepsSinceStarAttack;
	int starAttackBurstFiredCount;
	int starAttackBurstCount;
	int starAttackBurstThreshold;
	int stepsSinceStarAttackBurstFired;
	bool inStarAttack;

	void tryToMove();
	void step();
	void tryStartingStarAttack();
	void tryStarBurst();

public:
	Boss();
	Boss(int startingHealth);
	int getBossHealth();
	void setBossHealth(int health);
	int eventHandler(const df::Event* p_e);
	
};

