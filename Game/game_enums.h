#ifndef GAME_ENUMS_H
#define GAME_ENUMS_H

#include <chrono>

enum UserInput {
	NONE,
	m_LEFT,
	m_RIGHT,
	m_UP,
	m_DOWN
};

struct animVars {
	std::chrono::milliseconds animTime;
	std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> initTime;
	int currentFrame;
	int previousFrame;
	bool bounce;
};

#endif GAME_ENUMS_H