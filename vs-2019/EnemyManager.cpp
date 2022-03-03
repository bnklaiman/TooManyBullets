#include <EventStep.h>
#include <LogManager.h>
#include <random>

#include "EnemyManager.h"
#include "WorldManager.h"

// time in seconds when the music hits its high the point and the boss appears
// #define MUSIC_CLIMAX_TIME 71.9

#define MUSIC_CLIMAX_TIME 71.9

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

	if (stepsElapsed % 75 == 0) {
		randomSpawn();
	}
}

void EnemyManager::randomSpawn() {
	float chance = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (chance > 0.75) {
		Enemy* e = new Enemy();

		int worldWidth = (int)WM.getBoundary().getHorizontal();
		int worldHeight = (int)WM.getBoundary().getVertical();
		df::ObjectList ol;
		df::Vector loc;
		int tries = 0;
		do {
			int x = 2 + (rand() % static_cast<int>(worldWidth - 2 + 1));
			int y = 2 + (rand() % static_cast<int>(((1 * worldHeight) / 4) - 2 + 1));
			loc = df::Vector(x, y);
			ol = WM.getCollisions(e, loc);
			if (tries > 15) {
				WM.markForDelete(e);
				return;
			}
		} while (!ol.isEmpty());
		WM.moveObject(e, loc);
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