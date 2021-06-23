#include "physics.h"

void physics::applyForce(float* x, float* y, float force, UserInput direction, float dt) {
	switch (direction) {
	case m_LEFT:
		*x -= force * dt;
		break;
	case m_RIGHT:
		*x += force * dt;
		break;
	case m_UP:
		*y -= force * dt;
		break;
	case m_DOWN:
		*y += force * dt;
		break;
	}
}

bool physics::checkCollision(float obj1PosX, float obj1PosY, float obj1Width, float obj1Height, float obj2PosX, float obj2PosY, float obj2Width, float obj2Height) {
	if (obj1PosY + obj1Height > obj2PosY && obj1PosY < obj2PosY + obj2Height && obj1PosX + obj1Width > obj2PosX && obj1PosX < obj2PosX + obj2Width) {
		return true;
	}

	else return false;
}