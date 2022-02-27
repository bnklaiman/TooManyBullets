#include <EventKeyboard.h>
#include <EventStep.h>
#include <GameManager.h>
#include <LogManager.h>

#include "Enemy.h"
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
}

void GameStart::start() {
	Player* player = new Player();
	LM.writeLog("New player created.");
	new Enemy;
	LM.writeLog("New enemy created.");
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
