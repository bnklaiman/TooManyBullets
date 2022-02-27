#pragma once

#include "Object.h"
#include "EventKeyboard.h"

class Player : public df::Object {
private:
	void kbd(const df::EventKeyboard* p_keyboard_event);
	void move(float dx, float dy);
	void fire();
	int fireSlowdown;
	int fireCooldown;
	bool slowmode;

public:
	Player();
	int eventHandler(const df::Event* p_e) override;
};