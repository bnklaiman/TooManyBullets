#include <EventStep.h>
#include <LogManager.h>

#include "EnemyManager.h"

// time in seconds when the music hits its high the point and the boss appears
// #define MUSIC_CLIMAX_TIME 71.9

#define MUSIC_CLIMAX_TIME 1

EnemyManager::EnemyManager() {
	isBossPresent = false;
	stepsElapsed = 0;
	bossHealth = 0;
	p_boss_hp = nullptr;

	new Enemy;
	LM.writeLog("New enemy created.");
	registerInterest(df::STEP_EVENT);
}

int EnemyManager::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		stepsElapsed++;
		if (stepsElapsed >= (MUSIC_CLIMAX_TIME * 30) && !isBossPresent) {
			new Boss;
			LM.writeLog("New boss created.");
			isBossPresent = true;
			bossHealth = 5000;

			p_boss_hp = new df::ViewObject; // Count of lives
			p_boss_hp->setLocation(df::TOP_CENTER);
			p_boss_hp->setViewString("Boss:");
			p_boss_hp->setValue(bossHealth);
			p_boss_hp->setColor(df::RED);
		}

		if (isBossPresent && bossHealth <= 0) {
			isBossPresent = false;
			p_boss_hp = nullptr;
		}

		return 1;
	}

	return 0;
}