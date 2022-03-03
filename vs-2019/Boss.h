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

	int starAttackThreshold;			// how long since the last burst of stars until a new one can st art
	int stepsSinceStarAttack;			// how long its been since the last burst of stars
	int starAttackBurstFiredCount;		// how many stars have been fired in the current burst
	int starAttackBurstCount;			// how many stars are in a burst
	int starAttackBurstThreshold;		// time between stars in a burst
	int stepsSinceStarAttackBurstFired;	// how long its been since a star was fired in this burst
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

