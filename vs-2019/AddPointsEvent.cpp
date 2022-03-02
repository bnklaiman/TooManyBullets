#include "AddPointsEvent.h"

AddPointsEvent::AddPointsEvent() {
	pointsDelta = 0;
	setType(POINTS_ADD_EVENT);
}

AddPointsEvent::AddPointsEvent(int points) {
	pointsDelta = points;
	setType(POINTS_ADD_EVENT);
}

int AddPointsEvent::getPointsDelta() const {
	return pointsDelta;
}

void AddPointsEvent::setPointsDelta(int points) {
	pointsDelta = points;
}
