#include <EventStep.h>
#include <GameManager.h>
#include <LogManager.h>
#include <WorldManager.h>

#include "GameOver.h"
#include "GameStart.h"

GameOver::GameOver() {
	setType("GameOver");

	// Link to "message" sprite
	if (setSprite("gameover") == 0) {
		time_to_live = getAnimation().getSprite()->getFrameCount() * getAnimation().getSprite()->getSlowdown();
	}
	else {
		time_to_live = 0;
	}

	// Put in center of window
	setLocation(df::CENTER_CENTER);

	// Register for step event
	registerInterest(df::STEP_EVENT);
}

// When object exits, indicate game over
GameOver::~GameOver() {
	// Remove Enemies and ViewObjects, re-activate GameStart
	df::ObjectList object_list = WM.getAllObjects(true);
	df::ObjectListIterator i(&object_list);
	for (i.first(); !i.isDone(); i.next()) {
		df::Object* p_o = i.currentObject();
		if (p_o->getType() == "Enemy" || 
			p_o->getType() == "ViewObject" || 
			p_o->getType() == "Player" || 
			p_o->getType() == "EnemyManager" ||
			p_o->getType() == "Boss" ||
			p_o->getType() == "Bullet") {
			WM.markForDelete(p_o);
		}
		if (p_o->getType() == "GameStart") {
			LM.writeLog("GameStart reached!");
			p_o->setActive(true);
			LM.writeLog("isActive: %d", p_o->isActive());
			dynamic_cast<GameStart*>(p_o)->stopMusic(); // Stop music
		}
	}
}

int GameOver::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 1;
	}

	// If we get here, event ignored
	return 0;
}

// Count down to end of "message"
void GameOver::step() {
	time_to_live--;
	if (time_to_live <= 0) {
		WM.markForDelete(this);
	}
}

// Override default draw so as not to display "value"
int GameOver::draw() {
	return df::Object::draw();
}