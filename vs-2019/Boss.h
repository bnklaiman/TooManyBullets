#pragma once

#include <EventCollision.h>
#include <Object.h>

class Boss : public df::Object {
private:
	int bossHealth = 5000;

public:
	Boss();
	Boss(int startingHealth);
	int getBossHealth();
	void setBossHealth(int health);
};

