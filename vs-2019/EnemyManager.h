#pragma once

#include <Event.h>
#include <Object.h>
#include <ViewObject.h>

#include "Boss.h"
#include "Enemy.h"

class EnemyManager : public df::Object {
private:
	bool isBossPresent;
	int stepsElapsed;
	int bossHealth;
	df::ViewObject* p_boss_hp;

public:
	EnemyManager();
	int eventHandler(const df::Event* p_e) override;
};

