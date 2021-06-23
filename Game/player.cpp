#include "player.h"
using namespace std::chrono;

Player::Player() {
	this->playerState  = "DOWN_WALK1";
	this->currentInput = NONE;
	this->lastInput = NONE;
}

UserInput Player::GetPlayerAction() {
	return this->currentInput;
}

UserInput Player::GetLastPlayerAction() {
	return this->lastInput;
}

std::string Player::GetSprite() {
	return this->playerState;
};

int Player::GetCurrentFrame() {
	return this->currentFrame;
}

int Player::GetLastFrame() {
	return this->lastFrame;
}

float Player::GetPos0() {
	return this->pos[0];
}

float Player::GetPos1() {
	return this->pos[1];
}

float Player::GetSize0() {
	return this->size[0];
}

float Player::GetSize1() {
	return this->size[1];
}

void Player::UpdateState(UserInput newState) {
	this->lastInput = this->currentInput;

	switch (newState) {
	case L_WALK:
		this->playerState = "LEFT_WALK1";
		break;
	case R_WALK:
		this->playerState = "RIGHT_WALK1";
		break;
	case U_WALK:
		this->playerState = "UP_WALK1";
		break;
	case D_WALK:
		this->playerState = "DOWN_WALK1";
		break;
	case NONE:
		this->playerState.replace(this->playerState.size() - 1, 1, "2");
	}

	this->currentInput = newState;
};

void Player::UpdateSprite() {
	if (this->GetCurrentFrame() == 1) {
		this->currentFrame = 2;
		this->lastFrame = 1;
	}
	else if (this->GetCurrentFrame() == 3) {
		this->currentFrame = 2;
		this->lastFrame = 3;
	}
	else if (this->GetCurrentFrame() == 2 && this->GetLastFrame() == 1) {
		this->currentFrame = 3;
		this->lastFrame = 2;
	}
	else if (this->GetCurrentFrame() == 2 && this->GetLastFrame() == 3) {
		this->currentFrame = 1;
		this->lastFrame = 2;
	}
	
	this->frameNumber = std::to_string(this->GetCurrentFrame());
	this->playerState.replace(this->playerState.size() - 1, 1, frameNumber);
}