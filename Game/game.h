#ifndef GAME_H
#define GAME_H

#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <map>
#include <vector>

#include "game_enums.h"
#include "physics.h"
#include "Object.h"
#include "interactible.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "scenarios.h"

using namespace std::chrono;

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_PAUSE
};

class Game
{
public:
    GameState    State;
    bool         Keys[1024];
    unsigned int Width, Height;
    UserInput playerInput;
    glm::mat4 projection;

    float screenLeft, screenRight, screenTop, screenDown;
    
    std::map<std::string, Object*> gameObjects;
    std::map<std::string, Interactible*> gameInteractibles;

    Game(unsigned int width, unsigned int height);
    ~Game();
    
    milliseconds timeDifference;
    time_point<system_clock, milliseconds> inputTime  = time_point_cast<milliseconds>(system_clock::now());
    time_point<system_clock, milliseconds> updateTime = time_point_cast<milliseconds>(system_clock::now());
    
    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void Render();
    void moveCamera(float followX, float followY);
    void loadTexture(std::map<std::string, std::string> sprites, animVars animationVars, std::string animName, Object& obj);
    void initAnim(Object& obj, std::string animation);
    void runAnim(Object& obj, std::string animation);
    void setSprite(Object& obj, std::string animation, int frame);
    void readGameObjects();
    void readGameObjectsTextures();
    void readGameScenarios();
};

#endif