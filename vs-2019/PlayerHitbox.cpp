#include "PlayerHitbox.h"
#include "WorldManager.h"
#include "EventCollision.h"

PlayerHitbox::PlayerHitbox(Player* player) {
	setType("PlayerHitbox");
	parent = player;
	setSprite("PlayerHitbox");
	setPosition(parent->getPosition());
	setAltitude(df::MAX_ALTITUDE - 1);
	setSolidness(df::SOFT); // must be soft otherwise everything counts as a collision that prevents movement
}

void PlayerHitbox::hit(bool byBullet) {
	parent->hit(byBullet);
}

int PlayerHitbox::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* pCE = dynamic_cast<const df::EventCollision*> (p_e);
		std::string obj1 = pCE->getObject1()->getType();
		std::string obj2 = pCE->getObject2()->getType();
		if (obj1 == "Enemy" || obj1 == "Boss" || obj2 == "Enemy" || obj2 == "Boss") {
			// an enemy or boss hit the hitbox
			hit(false);
		}
		return 1;
	}
	return 0;
}
