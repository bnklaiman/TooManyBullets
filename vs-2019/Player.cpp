#include "LogManager.h"
#include "WorldManager.h"
#include "ResourceManager.h"
#include "DisplayManager.h"

#include "Bullet.h"
#include "Player.h"
#include "EventStep.h"
#include "HeroBullet.h"


Player::Player() {
	// Set up "player" sprite
	setSprite("player");

	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::STEP_EVENT);

	// Set object type
	setType("Player");
	df::Vector p(WM.getBoundary().getHorizontal() / 2,  3 * WM.getBoundary().getVertical() / 4);
	setPosition(p);

	fireSlowdown = 3;
	fireCooldown = fireSlowdown;
}

// Only respond to "keyboard event"
// to do so, cast generic Event object pointer as an EventKeyboard object pointer, then call kbd()
int Player::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::KEYBOARD_EVENT) {
		const df::EventKeyboard* p_keyboard_event = dynamic_cast<const df::EventKeyboard*> (p_e);
		kbd(p_keyboard_event);
		return 1;
	} else if (p_e->getType() == df::STEP_EVENT) {
		// create the illusion of rapid fire so it's not just a column of bullets (part 1)
		fireCooldown--;
		if (fireCooldown < 0) {
			fireCooldown = 0;
		}
	}
	// LM.writeLog("current step: %d", step);
	return 0;
}

// Take appropriate action according to key pressed
void Player::kbd(const df::EventKeyboard* p_keyboard_event) {
	float moveSpeed = 0.2;
	// if slowmode
		// movespeed *= 0.5
	float charWidth = df::charWidth(); //DM.getHorizontalPixels() / DM.getHorizontal();
	float charHeight = df::charHeight() / 3;  //DM.getVerticalPixels() / DM.getVertical();
	switch (p_keyboard_event->getKey()) {
	case df::Keyboard::W:
	case df::Keyboard::UPARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(0, -moveSpeed * charHeight);
		//LM.writeLog("moved: ^");
		break;
	case df::Keyboard::A:
	case df::Keyboard::LEFTARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(-moveSpeed * charWidth, 0);
		//LM.writeLog("moved: <");
		break;
	case df::Keyboard::S:
	case df::Keyboard::DOWNARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(0, moveSpeed * charHeight);
		//LM.writeLog("moved: v");
		break;
	case df::Keyboard::D:
	case df::Keyboard::RIGHTARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(moveSpeed * charWidth, 0);
		//LM.writeLog("moved: >");
		break;
	case df::Keyboard::Z:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) fire();
		//LM.writeLog("fired");
		break;
	}
}

// Move around
void Player::move(float dx, float dy) {
	// If stays on window, allow move
	df::Vector new_pos(getPosition().getX() + dx, getPosition().getY() + dy);
	if ((new_pos.getY() > 1) && (new_pos.getY() < WM.getBoundary().getVertical() - 1) &&
	   (new_pos.getX() > 1) && (new_pos.getX() < WM.getBoundary().getHorizontal() - 1)) {
		WM.moveObject(this, new_pos);
	}
}

void Player::fire() {
	// create the illusion of rapid fire so it's not just a column of bullets (part 2)
	//LM.writeLog("called fire");
	if (fireCooldown <= 0) {
		//LM.writeLog("fired");
		fireCooldown = fireSlowdown;
		df::Vector v = df::Vector(0, -3);
		HeroBullet* p = new HeroBullet(getPosition(), true);
		p->setVelocity(v);
	}
}