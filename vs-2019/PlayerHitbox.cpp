#include "PlayerHitbox.h"
#include "WorldManager.h"

PlayerHitbox::PlayerHitbox(Player* player) {
	setType("PlayerHitbox");
	parent = player;
	setSprite("PlayerHitbox");
	setPosition(parent->getPosition());
	setAltitude(df::MAX_ALTITUDE - 1);
	setSolidness(df::SOFT); // must be soft otherwise everything counts as a collision that prevents movement
}

void PlayerHitbox::hit() {
	parent->hit();
}
