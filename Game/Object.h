#ifndef OBJECT_H
#define OBJECT_H

#include <string>
#include <map>
#include <vector>
#include <iostream>

#include "game_enums.h"

class Object {
public:
	Object(float sizeX, float sizeY, float textureSizeX, float textureSizeY, float positionX, float positionY, bool canCollide, bool visible, bool repeatTexture);
	void includeAnim(std::string name, std::vector<std::string> sprites, struct animVars animationVars);
	std::vector<std::string> getAnim(std::string name);
	
	std::map<std::string, std::vector<std::string>> animations;
	std::map<std::string, struct animVars> animationVars;
	std::string currentSprite;
	std::string currentAnim = "NONE";
	float position[2], size[2], textureSize[2];
	bool canCollide;
	bool visible;
	bool repeatTexture;
};

#endif