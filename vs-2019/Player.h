#pragma once

#include <Object.h>
#include <EventKeyboard.h>

#include "PlayerHitbox.h"

class PlayerHitbox;

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
	
public:
	Player();
	~Player();
	int eventHandler(const df::Event* p_e) override;
	int getLivesRemaining();
	void setLivesRemaining(int lives);
	void hit();
	void graze();
};