#include "HeroBullet.h"

HeroBullet::HeroBullet(df::Vector initialPos, bool deleteOnOut):Bullet(initialPos, deleteOnOut) {
	setType("HeroBullet");
	setSprite("herobullet");
}
