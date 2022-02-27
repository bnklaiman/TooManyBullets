#pragma once

#include "Bullet.h"

class HeroBullet : public Bullet {
public:
    HeroBullet(df::Vector initialPos, bool deleteOnOut);
};

