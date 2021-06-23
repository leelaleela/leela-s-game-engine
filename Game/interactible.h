#include <string>
#include "Object.h"
#include "Physics.h"

class Interactible{
public:
	Interactible(Object* obj);
	void follow(float followX, float followY, bool collides, float dt, physics& gravity);

	Object* object;
	std::vector<void(*)()> interactions;
	bool isActive;
};