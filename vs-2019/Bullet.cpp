#include <EventOut.h>
#include <EventStep.h>
#include <LogManager.h>
#include <WorldManager.h>

#include "Bullet.h"
#include "Explosion.h"
#include "GameOver.h"

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

// If Bullet moves outside world, mark self for deletion
void Bullet::out() {
	WM.markForDelete(this);
}

// If Bullet hits enemy, mark Enemy and Bullet for deletion
void Bullet::hit(const df::EventCollision* p_collision_event) {
	
	if (((p_collision_event->getObject1()->getType() == "Enemy") ||
		(p_collision_event->getObject2()->getType() == "Enemy")) 
		&& shooter != "Enemy") {
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
		
		if (p_collision_event->getObject1()->getType() == "Enemy") {
			Explosion* p_explosion = new Explosion;
			p_explosion->setPosition(p_collision_event->getObject1()->getPosition());
		} else if (p_collision_event->getObject2()->getType() == "Enemy") {
			Explosion* p_explosion = new Explosion;
			p_explosion->setPosition(p_collision_event->getObject2()->getPosition());
		}
	}

	if (p_collision_event->getObject1()->getType() == "Player" || p_collision_event->getObject2()->getType() == "Player" && shooter == "Enemy") {
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());

		if (p_collision_event->getObject1()->getType() == "Player") {
			Explosion* p_explosion = new Explosion;
			p_explosion->setPosition(p_collision_event->getObject1()->getPosition());
		} else if (p_collision_event->getObject2()->getType() == "Player") {
			Explosion* p_explosion = new Explosion;
			p_explosion->setPosition(p_collision_event->getObject2()->getPosition());
		}

		new GameOver;
	}
}