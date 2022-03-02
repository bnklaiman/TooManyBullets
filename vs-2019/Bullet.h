#pragma once
#include <Object.h>
#include <EventCollision.h>

#include <string>

class Bullet : public df::Object {
private:
	void out();
	void hit(const df::EventCollision* p_collision_event);
	bool deleteOnOut;
	bool hasGrazed;

public:
	std::string shooter;
	Bullet(df::Vector initialPosition, bool newDeleteOnOut=false);
	int eventHandler(const df::Event* p_e) override;
	bool getHasGrazed() const;
	void setHasGrazed(bool grazed);
};

