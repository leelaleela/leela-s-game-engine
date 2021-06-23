#ifndef PLAYER_H
#define PLAYER_H

#include <chrono>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "game_enums.h"

class Player {
public:

	Player();

	std::string GetSprite();

	UserInput GetPlayerAction();
	UserInput GetLastPlayerAction();

	int GetCurrentFrame();
	int GetLastFrame();

	float* GetPos();
	float GetPos0();
	float GetPos1();

	float* GetSize();
	float GetSize0();
	float GetSize1();

	void UpdateState(UserInput newState);

	void UpdateSprite();

	std::string playerState;
	std::string frameNumber;
	UserInput currentInput;
	UserInput lastInput;

	float pos[2];
	float size[2];
	int health = 100, stamina = 100, guard = 100, currentFrame = 1, lastFrame = 2;
};

#endif PLAYER_H
