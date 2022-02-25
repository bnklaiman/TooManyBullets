#include "Bullet.h"
#include "EventStep.h"
#include "LogManager.h"
#include "WorldManager.h"

Bullet::Bullet(df::Vector hero_pos) {
	setSprite("bullet");
	setType("Bullet");

	// Set starting location, based on hero's position passed in
	df::Vector p(hero_pos.getX(), hero_pos.getY() - 2);
	setPosition(p);

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
	} else if (p_e->getType() == df::STEP_EVENT) {
		if (getPosition().getY() < 0 || getPosition().getY() > WM.getBoundary().getVertical()) {
			out();
		}
	}
	return 0;
}

// If Bullet moves outside world, mark self for deletion
void Bullet::out() {
	WM.markForDelete(this);
	LM.writeLog("Bullet marked for deletion.");
}

// If Bullet hits enemy, mark Enemy and Bullet for deletion
void Bullet::hit(const df::EventCollision* p_collision_event) {
	if ((p_collision_event->getObject1()->getType() == "Enemy") ||
		(p_collision_event->getObject2()->getType() == "Enemy")) {
		WM.markForDelete(p_collision_event->getObject1());
		WM.markForDelete(p_collision_event->getObject2());
	}
}