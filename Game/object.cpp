#include "Object.h"

Object::Object(float sizeX, float sizeY, float textureSizeX, float textureSizeY, float positionX, float positionY, bool canCollide, bool visible, bool repeatTexture) {
	this->position[0] = positionX;
	this->position[1] = positionY;
	this->size[0] = sizeX;
	this->size[1] = sizeY;
	this->textureSize[0] = textureSizeX;
	this->textureSize[1] = textureSizeY;
	this->canCollide = canCollide;
	this->visible = visible;
	this->repeatTexture = repeatTexture;
}

void Object::includeAnim(std::string name, std::vector<std::string> sprites, struct animVars animationVars) {
	this->animations.insert(std::pair<std::string, std::vector<std::string>>(name, sprites));
	this->animationVars.insert(std::pair<std::string, struct animVars>(name, animationVars));
}

std::vector<std::string> Object::getAnim(std::string name) {
	return animations.at(name);
}