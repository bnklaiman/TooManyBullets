#pragma once

#include <EventCollision.h>
#include <Object.h>

class Enemy : public df::Object {
public:
	Enemy();
private:
	int stepsSinceLastAttack;	// how many steps since we last attacked
	int attackThreshold;		// threshold of steps since last attack before we can attack again
	int moveStepsRemaining;		// how many steps we should move for
	int stepsSinceMove;			// how many steps since we last moved
	int moveThreshold;			// threshold of steps since last move before we can move again
	void basicAttack();
	void shotgunAttack();
	void biggerShotgun();
	int eventHandler(const df::Event* p_e) override;
	void step();
	void tryToMove();
};