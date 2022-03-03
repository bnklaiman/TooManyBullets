#include <EventStep.h>
#include <EventView.h>
#include <LogManager.h>
#include <WorldManager.h>
#include <ResourceManager.h>
#include <DisplayManager.h>
#include <Sound.h>

#include "Bullet.h"
#include "Explosion.h"
#include "GameOver.h"
#include "HeroBullet.h"
#include "Player.h"
#include "AddPointsEvent.h"

Player::Player() {
	// Set up "player" sprite
	setSprite("player");

	registerInterest(df::KEYBOARD_EVENT);
	registerInterest(df::STEP_EVENT);
	registerInterest(POINTS_ADD_EVENT);


	// Set object type
	setType("Player");
	df::Vector p(WM.getBoundary().getHorizontal() / 2,  3 * WM.getBoundary().getVertical() / 4);
	setPosition(p);
	setSolidness(df::SOFT);

	fireSlowdown = 3;
	fireCooldown = fireSlowdown;

	slowmode = false;

	livesRemaining = 3;
	score = 0;

	hitbox = new PlayerHitbox(this);

	iframes = 0;
}

Player::~Player() {
	WM.markForDelete(hitbox);
	new GameOver;
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
		// flashing effect for iframes
		if (iframes > 0) {
			if((iframes < 80 && iframes > 70) ||
				(iframes < 60 && iframes > 50) ||
				(iframes < 40 && iframes > 30) ||
				(iframes < 20 && iframes > 10)) {
				setVisible(false);
			} else {
				setVisible(true);
			}
			iframes--;
		}
		return 1;
	} else if (p_e->getType() == POINTS_ADD_EVENT) {
		const AddPointsEvent* pAE = dynamic_cast<const AddPointsEvent*> (p_e);
		setScore(getScore() + pAE->getPointsDelta());
		return 1;
	}
	// LM.writeLog("current step: %d", step);
	return 0;
}

// Take appropriate action according to key pressed
void Player::kbd(const df::EventKeyboard* p_keyboard_event) {
	float moveSpeed = 0.15;
	if (slowmode) moveSpeed *= 0.5;
	float charWidth = df::charWidth(); //DM.getHorizontalPixels() / DM.getHorizontal();
	float charHeight = df::charHeight() / 3;  //DM.getVerticalPixels() / DM.getVertical();
	switch (p_keyboard_event->getKey()) {
	case df::Keyboard::W:
	case df::Keyboard::UPARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(0, -moveSpeed * charHeight);
		// LM.writeLog("^");
		break;
	case df::Keyboard::A:
	case df::Keyboard::LEFTARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(-moveSpeed * charWidth, 0);
		// LM.writeLog("<");
		break;
	case df::Keyboard::S:
	case df::Keyboard::DOWNARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(0, moveSpeed * charHeight);
		// LM.writeLog(">");
		break;
	case df::Keyboard::D:
	case df::Keyboard::RIGHTARROW:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) move(moveSpeed * charWidth, 0);
		// LM.writeLog("v");
		break;
	case df::Keyboard::Z:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) fire();
		// LM.writeLog("Z");
		break;
	case df::Keyboard::Q:
		// quit
		if (p_keyboard_event->getKeyboardAction() == df::KEY_PRESSED) WM.markForDelete(this);
		// LM.writeLog("Q");
		break;
	case df::Keyboard::LEFTSHIFT:
	case df::Keyboard::RIGHTSHIFT:
		if (p_keyboard_event->getKeyboardAction() == df::KEY_DOWN) slowmode = true;
		if (p_keyboard_event->getKeyboardAction() == df::KEY_RELEASED) slowmode = false;
		// LM.writeLog("SHIFT");
	}
}

// Move around
void Player::move(float dx, float dy) {
	// If stays on window, allow move
	df::Vector oldPos = getPosition();
	df::Vector new_pos(oldPos.getX() + dx, oldPos.getY() + dy);
	/*WM.moveObject(this, new_pos);
	if (!df::boxContainsBox(WM.getBoundary(), df::getWorldBox(this))) {
		WM.moveObject(this, oldPos);
	}*/
	if ((new_pos.getY() > 1) && (new_pos.getY() < WM.getBoundary().getVertical() - (getBox().getVertical() / 2)) &&
	   (new_pos.getX() > 1) && (new_pos.getX() < WM.getBoundary().getHorizontal() - (getBox().getHorizontal() / 2))) {
		WM.moveObject(this, new_pos);
		WM.moveObject(hitbox, getPosition());
	}
}

void Player::fire() {
	// create the illusion of rapid fire so it's not just a column of bullets (part 2)
	//LM.writeLog("called fire");
	if (fireCooldown <= 0) {
		//LM.writeLog("fired");
		fireCooldown = fireSlowdown;
		df::Vector v = df::Vector(0, -2);
		HeroBullet* p = new HeroBullet(getPosition(), true);

		// Play "fire" sound
		df::Sound* p_sound = RM.getSound("fire");
		p_sound->play();

		p->setVelocity(v);
		df::Vector p1 = getPosition();
		df::Vector p2(p1);
		int dx = 1.5;
		if (slowmode) dx = 3;
		p1 += df::Vector(-dx, -1);
		p2 += df::Vector(dx, -1);
		Bullet* s1 = new Bullet(p1, true);
		Bullet* s2 = new Bullet(p2, true);
		if (!slowmode) {
			df::Vector v1 = df::Vector(-2.5, -1);
			df::Vector v2 = df::Vector(2.5, -1);
			s1->setDirection(v1);
			s2->setDirection(v2);
		} else {
			df::Vector d = df::Vector(0, -1);
			s1->setDirection(d);
			s2->setDirection(d);
		}
		s1->setSpeed(2);
		s2->setSpeed(2);
		s1->setSprite("circleflashbullet");
		s2->setSprite("circleflashbullet");
		p->shooter = getType();
		s1->shooter = getType();
		s2->shooter = getType();
		p->setAltitude(3);
		s1->setAltitude(3);
		s2->setAltitude(3);
	}
}

void Player::hit(bool byBullet) {
	if (iframes > 0) return;
	Explosion* p_explosion = new Explosion;
	p_explosion->setPosition(getPosition());
	// Play "player hit" sound
	df::Sound* p_sound = RM.getSound("playerhit");
	p_sound->play();
	if(byBullet) setScore(getScore() - 25);

	if (getLivesRemaining() > 0) {
		setLivesRemaining(getLivesRemaining() - 1);
		df::Vector p(WM.getBoundary().getHorizontal() / 2, 3 * WM.getBoundary().getVertical() / 4);
		setPosition(p);
		hitbox->setPosition(getPosition());
		iframes = 90;
	} else {
		WM.markForDelete(this);
		WM.markForDelete(hitbox);
	}
}

int Player::getLivesRemaining() {
	return livesRemaining;
}

void Player::setLivesRemaining(int lives) {
	livesRemaining = lives;
	df::EventView ev("Lives:", lives, false);
	WM.onEvent(&ev);
}

void Player::graze() {
	setScore(getScore() + GRAZE_POINTS);
}

int Player::getScore() {
	return score;
}

void Player::setScore(int newScore) {
	score = newScore;
	df::EventView ev("Score:", newScore, false);
	WM.onEvent(&ev);
}
