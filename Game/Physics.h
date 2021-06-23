#ifndef PHYSICS_H
#define PHYSICS_H

#include "game_enums.h"

class physics
{
public:
	void applyForce(float* x, float* y, float force, UserInput direction, float dt);
	bool checkCollision(float obj1PosX, float obj1PosY, float obj1Width, float obj1Height, float obj2PosX, float obj2PosY, float obj2Width, float obj2Height);
};
#endif PHYSICS_H