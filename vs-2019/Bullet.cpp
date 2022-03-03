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
	setAltitude(2);

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
		df::Sound* p_sound;
		Explosion* p_explosion;

		if (p_collision_event->getObject1()->getType() == "Enemy") {
			p_explosion = new Explosion;

			// Play "enemy hit" sound
			p_sound = RM.getSound("enemyhit");
			p_sound->play();

			p_explosion->setPosition(p_collision_event->getObject1()->getPosition());

			
		} else if (p_collision_event->getObject2()->getType() == "Enemy") {
			p_explosion = new Explosion;

			// Play "enemy hit" sound
			p_sound = RM.getSound("enemyhit");
			p_sound->play();

			p_explosion->setPosition(p_collision_event->getObject2()->getPosition());
		}
		AddPointsEvent* pAE = new AddPointsEvent(ENEMY_POINT_VALUE);
		WM.onEvent(pAE);
	}

	// a bullet hit a boss
	Boss* b;
	Explosion* p_explosion;
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
			WM.markForDelete(b);

			Explosion* p_explosion = new Explosion;
			Explosion* p_explosion1 = new Explosion;
			Explosion* p_explosion2 = new Explosion;
			Explosion* p_explosion3 = new Explosion;
			Explosion* p_explosion4 = new Explosion;
			Explosion* p_explosion5 = new Explosion;
			Explosion* p_explosion6 = new Explosion;
			Explosion* p_explosion7 = new Explosion;
			Explosion* p_explosion8 = new Explosion;
			df::Sound* p_sound;

			if (p_collision_event->getObject1()->getType() == "Boss") {
				p_sound = RM.getSound("bossdown");
				p_sound->play();
				p_explosion->setPosition(p_collision_event->getObject1()->getPosition());
				p_explosion1->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(0, -2));
				p_explosion2->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(3, -3));
				p_explosion3->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(5, 0));
				p_explosion4->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(3, 3));
				p_explosion5->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(0, 2));
				p_explosion6->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(-3, 3));
				p_explosion7->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(-5, 0));
				p_explosion8->setPosition(p_collision_event->getObject1()->getPosition() + df::Vector(-3, -3));
			} else if (p_collision_event->getObject2()->getType() == "Boss") {
				p_sound = RM.getSound("bossdown");
				p_sound->play();
				p_explosion->setPosition(p_collision_event->getObject2()->getPosition());
				p_explosion1->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(0, -2));
				p_explosion2->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(3, -3));
				p_explosion3->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(5, 0));
				p_explosion4->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(3, 3));
				p_explosion5->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(0, 2));
				p_explosion6->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(-3, 3));
				p_explosion7->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(-5, 0));
				p_explosion8->setPosition(p_collision_event->getObject2()->getPosition() + df::Vector(-3, -3));
			}
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
		pb->hit(true);
	}
}