#pragma once

#include <ViewObject.h>

class GameOver : public df::ViewObject {
private:
	int time_to_live;

public:
	GameOver();
	~GameOver();
	int eventHandler(const df::Event* p_e) override;
	void step();
	int draw() override;
};

