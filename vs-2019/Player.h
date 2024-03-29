#pragma once

#include <Object.h>
#include <EventKeyboard.h>

#include "PlayerHitbox.h"

class PlayerHitbox;

const int GRAZE_POINTS = 25;

class Player : public df::Object {
private:
	void kbd(const df::EventKeyboard* p_keyboard_event);
	void move(float dx, float dy);
	void fire();
	int fireSlowdown;
	int fireCooldown;
	bool slowmode;
	int livesRemaining;
	PlayerHitbox* hitbox;
	int iframes;
	int bombs;
	
	int score;
	void bomb();

public:
	Player();
	~Player();
	void hit(bool byBullet);
	int eventHandler(const df::Event* p_e) override;
	int getLivesRemaining();
	void setLivesRemaining(int lives);
	void graze();
	int getScore();
	void setScore(int newScore);
	int getBombs() const;
	
};