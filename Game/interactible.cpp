#include "interactible.h"

Interactible::Interactible(Object* obj) {
	this->object = obj;
}

void Interactible::follow(float followX, float followY, bool collides, float dt, physics& gravity) {
    std::string followAnim = this->object->currentAnim;

    if (!(collides)) {
        if (followX < this->object->position[0] - 1) {
            gravity.applyForce(&this->object->position[0], &this->object->position[1], 100.0f, m_LEFT, dt);
            followAnim = "LEFT_WALK";
        }

        else if (followX > this->object->position[0] + 1) {
            gravity.applyForce(&this->object->position[0], &this->object->position[1], 100.0f, m_RIGHT, dt);
            followAnim = "RIGHT_WALK";
        }

        if (followY > (this->object->position[1] + this->object->size[1]) + 1) {
            gravity.applyForce(&this->object->position[0], &this->object->position[1], 100.0f, m_DOWN, dt);
            followAnim = "DOWN_WALK";
        }

        else if (followY < (this->object->position[1] + this->object->size[1]) - 1) {
            gravity.applyForce(&this->object->position[0], &this->object->position[1], 100.0f, m_UP, dt);
            followAnim = "UP_WALK";
        }

        if (this->object->currentAnim != followAnim) {
            this->object->animationVars.at(followAnim).initTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
            this->object->animationVars.at(followAnim).currentFrame = 0;
            this->object->animationVars.at(followAnim).previousFrame = 0;
            this->object->currentSprite = this->object->animations.at(followAnim).at(0);
            this->object->currentAnim = followAnim;
        }
    }

    else {
        if (this->object->currentAnim != "NONE") {
            this->object->currentSprite = this->object->animations.at(followAnim).at(1);
            this->object->currentAnim = "NONE";
        }
    }
}