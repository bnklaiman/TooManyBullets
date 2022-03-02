#include <EventKeyboard.h>
#include <EventStep.h>
#include <GameManager.h>
#include <LogManager.h>
#include <ResourceManager.h>
#include <ViewObject.h>

#include "Boss.h"
#include "Enemy.h"
#include "EnemyManager.h"
#include "GameStart.h"
#include "Player.h"

GameStart::GameStart() {
	setType("GameStart");
	setSprite("gamestart");

	// Put in center of window
	setLocation(df::CENTER_CENTER);

	// Register for step event
	registerInterest(df::STEP_EVENT);
	registerInterest(df::KEYBOARD_EVENT);

	// Set music and pause. (we only want to play during gameplay (i.e. the inverse of Saucer Shoot)
	p_music = RM.getMusic("music");
	stopMusic();
}

void GameStart::start() {
	playMusic();

	Player* player = new Player();
	LM.writeLog("New player created.");
	new EnemyManager(GM.getStepCount());

	df::ViewObject* p_lives = new df::ViewObject; // Count of lives
	p_lives->setLocation(df::TOP_LEFT);
	p_lives->setViewString("Lives:");
	p_lives->setValue(player->getLivesRemaining());
	p_lives->setColor(df::WHITE);

	df::ViewObject* p_score = new df::ViewObject; // Score
	p_score->setLocation(df::TOP_RIGHT);
	p_score->setViewString("Score:");
	p_score->setValue(player->getScore());
	p_score->setColor(df::YELLOW);

	setActive(false);
	LM.writeLog("GameStart set to inactive.");
}

int GameStart::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::KEYBOARD_EVENT) {
		df::EventKeyboard* p_keyboard_event = (df::EventKeyboard*)p_e;
		switch (p_keyboard_event->getKey()) {
		case df::Keyboard::P:
			// play
			start();
			break;
		case df::Keyboard::Q:
			// quit
			GM.setGameOver();
			break;
		default:
			// Key is not handled
			break;
		}
		return 1;
	}

	// If we get here, event ignored
	return 0;
}

// Override default draw so as not to display "value"
int GameStart::draw() {
	return df::Object::draw();
}

void GameStart::playMusic() {
	p_music->play();
}

void GameStart::stopMusic() {
	p_music->stop();
}