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
	int bossStartingHealth;
	df::ViewObject* p_boss_hp;
	int stepsAdjustment;
	bool bossNotSpawnedYet;
	void gameScript();

	void spawnBoss();

	void cleanUpAfterBoss();

public:
	EnemyManager(int stepsAdjust);
	int eventHandler(const df::Event* p_e) override;
};

