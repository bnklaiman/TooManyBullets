#pragma once
#include "Event.h"

const std::string POINTS_ADD_EVENT = "points_add";


class AddPointsEvent : public df::Event {
private:
	int pointsDelta;
public:
	AddPointsEvent();
	AddPointsEvent(int points);
	int getPointsDelta() const;
	void setPointsDelta(int points);
};

