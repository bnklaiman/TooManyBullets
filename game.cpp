//
// game.cpp
// 

// Engine includes.
#include "DisplayManager.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"

#include "vs-2019/Player.h"

void loadResources() {
	// Load player character sprite
	RM.loadSprite("sprites/player-chr.txt", "player");
}

void populateWorld() {
	Player* player = new Player();
}

int main(int argc, char* argv[]) {

	DM.startUp();
	DM.getWindow()->setTitle("Sample Text");

	// Start up game manager.
	if (GM.startUp()) {
		LM.writeLog("Error starting game manager!");
		GM.shutDown();
		return 0;
	}

	// Set flush of logfile during development (when done, make false).
	LM.setFlush(true);

	// ~~Show splash screen.~~ (nah, not yet)
	// df::splash();

	// Load game resources
	loadResources();

	// Populate game world with objects
	populateWorld();

	GM.run();

	// Shut everything down.
	GM.shutDown();
	DM.shutDown();
}