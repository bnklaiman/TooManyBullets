#pragma once

#include <ViewObject.h>

class GameStart : public df::ViewObject {
private:
	void start();

public:
	GameStart();
	int eventHandler(const df::Event* p_e) override;
	int draw() override;
};