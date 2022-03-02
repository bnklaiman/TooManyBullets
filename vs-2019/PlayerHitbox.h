#pragma once
#include "Object.h"
#include "Player.h"

class Player;

class PlayerHitbox : public df::Object {
private:
	Player* parent;
public:
	PlayerHitbox(Player* player);
	void hit();
};

