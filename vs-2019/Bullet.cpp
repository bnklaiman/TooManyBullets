#include <EventOut.h>
#include <EventStep.h>
#include <EventView.h>
#include <LogManager.h>
#include <ResourceManager.h>
#include <Sound.h>
#include <WorldManager.h>

#include "Boss.h"
#include "Bullet.h"
#include "Explosion.h"
#include "GameOver.h"
#include "Player.h"
#include "AddPointsEvent.h"

#define ENEMY_POINT_VALUE 100

Bullet::Bullet(df::Vector initialPosition, bool newDeleteOnOut) {
	setSprite("bullet");
	setType("Bullet");

	// Set starting location, based on hero's position passed in
	df::Vector p(initialPosition.getX(), initialPosition.getY());
	setPosition(p);
	deleteOnOut = newDeleteOnOut;
	// Bullets move each game loop
	// The direction is set when the hero fires
	
	// Make the Bullets soft so can pass through stuff
	setSolidness(df::SOFT);
	hasGrazed = false;
}

int Bullet::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::COLLISION_EVENT) {
		const df::EventCollision* p_collision_event = dynamic_cast<const df::EventCollision*> (p_e);
		hit(p_collision_event);
		return 1;
	} else if (p_e->getType() == df::OUT_EVENT && deleteOnOut) {
		out();
		return 1;
	}
	return 0;
}

bool Bullet::getHasGrazed() const {
	return hasGrazed;
}

void Bullet::setHasGrazed(bool grazed) {
	hasGrazed = grazed;
}

// If Bullet moves outside world, mark self for deletion
void Bullet::out() {
	WM.markForDelete(this);
}

// If Bullet hits enemy, mark Enemy and Bullet for deletion
void Bullet::hit(const df::EventCollision* p_collision_event) {
	
	// a bullet hit an enemy
	if (((p_collision_event->getObject1()->getType() == "Enemy") ||
		(p_collision_event->getObject2()->getType() == "Enemy")) 
		&& shooter != "Enemy") {
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
		
		Player* p;
		if (p_collision_event->getObject1()->getType() == "Enemy") {
			Explosion* p_explosion = new Explosion;

			// Play "enemy hit" sound
			df::Sound* p_sound = RM.getSound("enemyhit");
			p_sound->play();

			p_explosion->setPosition(p_collision_event->getObject1()->getPosition());

			
		} else if (p_collision_event->getObject2()->getType() == "Enemy") {
			Explosion* p_explosion = new Explosion;

			// Play "enemy hit" sound
			df::Sound* p_sound = RM.getSound("enemyhit");
			p_sound->play();

			p_explosion->setPosition(p_collision_event->getObject2()->getPosition());
		}
		AddPointsEvent* pAE = new AddPointsEvent(ENEMY_POINT_VALUE);
		WM.onEvent(pAE);
	}

	// a bullet hit a boss
	Boss* b;
	// something in general hit a boss... this and the above can be compacted
	if (((p_collision_event->getObject1()->getType() == "Boss") ||
		(p_collision_event->getObject2()->getType() == "Boss"))
		&& shooter != "Enemy") {
		
		if (p_collision_event->getObject1()->getType() == "Boss") {
			b = dynamic_cast<Boss*>(p_collision_event->getObject1());
			WM.markForDelete(p_collision_event->getObject2());
		}
		else if (p_collision_event->getObject2()->getType() == "Boss") {
			b = dynamic_cast<Boss*>(p_collision_event->getObject2());
			WM.markForDelete(p_collision_event->getObject1());
		}

		b->setBossHealth(b->getBossHealth() - 100);
		if (b->getBossHealth() <= 0) {
			LM.writeLog("Boss health depleted!");
		}

		
		if (b->getBossHealth() <= 0) {
			WM.markForDelete(b);
		}
	}

	// player grazed a bulet
	Player* p;
	Bullet* bu;
	if ((p_collision_event->getObject1()->getType() == "Player" || p_collision_event->getObject2()->getType() == "Player") && shooter != "Player") {
		if (p_collision_event->getObject1()->getType() == "Player") {
			p = dynamic_cast<Player*>(p_collision_event->getObject1());
			bu = dynamic_cast<Bullet*>(p_collision_event->getObject2());
		} else if (p_collision_event->getObject2()->getType() == "Player") {
			p = dynamic_cast<Player*>(p_collision_event->getObject2());
			bu = dynamic_cast<Bullet*>(p_collision_event->getObject1());
		}
		if (!bu->getHasGrazed()) {
			bu->setHasGrazed(true);
			p->graze();
		}
	}

	// player hitbox got hit 
	PlayerHitbox* pb;
	if ((p_collision_event->getObject1()->getType() == "PlayerHitbox" || p_collision_event->getObject2()->getType() == "PlayerHitbox") && shooter != "Player") {
		if (p_collision_event->getObject1()->getType() == "PlayerHitbox") {
			WM.markForDelete(p_collision_event->getObject2());
			pb = dynamic_cast<PlayerHitbox*>(p_collision_event->getObject1());
		}
		else if (p_collision_event->getObject2()->getType() == "PlayerHitbox") {
			WM.markForDelete(p_collision_event->getObject1());
			pb = dynamic_cast<PlayerHitbox*>(p_collision_event->getObject2());
		}
		pb->hit();
	}
}