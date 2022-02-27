#include <EventStep.h>
#include <WorldManager.h>

#include "Explosion.h"

Explosion::Explosion() {
	// Link to "explosion" sprite
	if (setSprite("explosion") == 0) {
		// Set live time to as long as sprite length
		time_to_live = getAnimation().getSprite()->getFrameCount();
	} else {
		time_to_live = 0;
	}

	setSolidness(df::SPECTRAL);
	registerInterest(df::STEP_EVENT);
}

int Explosion::eventHandler(const df::Event* p_e) {
	if (p_e->getType() == df::STEP_EVENT) {
		step();
		return 0;
	}
	
	return -1;
}

void Explosion::step() {
	time_to_live--;
	if (time_to_live <= 0) {
		WM.markForDelete(this);
	}
}