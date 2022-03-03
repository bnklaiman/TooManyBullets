#include <EventStep.h>
#include <LogManager.h>

#include "EnemyManager.h"
#include "WorldManager.h"

// time in seconds when the music hits its high the point and the boss appears
// #define MUSIC_CLIMAX_TIME 71.9

#define MUSIC_CLIMAX_TIME 1

EnemyManager::EnemyManager(int stepsAdjust) {
	setType("EnemyManager");
	isBossPresent = false;
	stepsElapsed = 0;
	bossStartingHealth = 5000;
	p_boss_hp = nullptr;
	stepsAdjustment = stepsAdjust;
	bossNotSpawnedYet = true;

	new Enemy;
	LM.writeLog("New enemy created.");
	registerInterest(df::STEP_EVENT);
}

int EnemyManager::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		gameScript();
		return 1;
	}
	return 0;
}

void EnemyManager::gameScript() {
	stepsElapsed++;
	if (stepsElapsed >= (MUSIC_CLIMAX_TIME * 30) && !isBossPresent && bossNotSpawnedYet) {
		spawnBoss();
	}
	if (isBossPresent && p_boss_hp->getValue() <= 0) {
		cleanUpAfterBoss();
	}
}

void EnemyManager::spawnBoss() {
	new Boss(bossStartingHealth);
	LM.writeLog("New boss created.");
	isBossPresent = true;
	bossNotSpawnedYet = false;

	p_boss_hp = new df::ViewObject; // Count of lives
	p_boss_hp->setLocation(df::BOTTOM_CENTER);
	p_boss_hp->setViewString("Boss:");
	p_boss_hp->setValue(bossStartingHealth);
	p_boss_hp->setColor(df::RED);
}

void EnemyManager::cleanUpAfterBoss() {
	isBossPresent = false;
	p_boss_hp->setVisible(false);
	WM.markForDelete(p_boss_hp);
	LM.writeLog("Should have deleted the boss hp ViewObject");
}