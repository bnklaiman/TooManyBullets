// Engine includes.
#include "DisplayManager.h"
#include "GameManager.h"
#include "LogManager.h"
#include "ResourceManager.h"
#include "WorldManager.h"

#include "vs-2019/Enemy.h"
#include "vs-2019/Player.h"

#include <direct.h>

void loadResources() {
	RM.loadSprite("sprites/player-chr.txt", "player");
	RM.loadSprite("sprites/enemy-chr.txt", "enemy");
	RM.loadSprite("sprites/bullet-spr.txt", "bullet");
}

void populateWorld() {
	Player* player = new Player();
	new Enemy;
}

int main(int argc, char* argv[]) {	
	LM.setLogLevel(1);
	// Start up game manager.
	if (GM.startUp()) {
		char tmp[256];
		_getcwd(tmp, 256);
		LM.writeLog("Working directory: %s", tmp);
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
}