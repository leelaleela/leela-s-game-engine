#include "game.h"
#include "test.cpp"

SpriteRenderer* Renderer;
physics gravity;

Game::Game(unsigned int width, unsigned int height)
    : State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{

}

Game::~Game()
{

}

void Game::Init()
{
    ResourceManager::LoadShader("vertshader.vert", "fragshader.vert", nullptr, "sprite");
    
    this->screenTop = 0.0f;
    this->screenLeft = 0.0f;
    this->screenDown = this->Height;
    this->screenRight = this->Width;

    this->projection = glm::ortho(0.0f, static_cast<float>(this->Width),
        static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", this->projection);
    
    Shader theShader = ResourceManager::GetShader("sprite");

    Renderer = new SpriteRenderer(theShader);
    
    this->readGameObjects();
    this->readGameObjectsTextures();
    //this->readGameScenarios();
    this->initAnim(*this->gameObjects.at("bipsi"), "DANCE");
}

void Game::Update(float dt)
{
    this->updateTime = time_point_cast<milliseconds>(system_clock::now());
    this->timeDifference = (this->updateTime - this->inputTime);

    this->moveCamera(this->gameObjects.at("hooman")->position[0], this->gameObjects.at("hooman")->position[1]);

    std::map<std::string, Object*>::iterator it = this->gameObjects.begin();

    for (std::pair<std::string, Object*> element : this->gameObjects) {
        if (element.second->currentAnim != "NONE") {
            this->runAnim(*element.second, element.second->currentAnim);
        }
    }

    this->gameInteractibles.at("maria")->follow(this->gameObjects.at("hooman")->position[0], this->gameObjects.at("hooman")->position[1] + this->gameObjects.at("hooman")->size[1], gravity.checkCollision(this->gameObjects.at("hooman")->position[0], this->gameObjects.at("hooman")->position[1], this->gameObjects.at("hooman")->size[0], this->gameObjects.at("hooman")->size[1], this->gameObjects.at("maria")->position[0], this->gameObjects.at("maria")->position[1], this->gameObjects.at("maria")->size[0], this->gameObjects.at("maria")->size[1]), dt, gravity);

    

    if (this->playerInput != NONE) {
        it = this->gameObjects.begin();

        gravity.applyForce(&this->gameObjects.at("hooman")->position[0], &this->gameObjects.at("hooman")->position[1], 100.0f, this->playerInput, dt);

        for (std::pair<std::string, Object*> element : this->gameObjects) {
            if (element.second->canCollide && element.first != "hooman") {
                bool result = gravity.checkCollision(this->gameObjects.at("hooman")->position[0], this->gameObjects.at("hooman")->position[1], this->gameObjects.at("hooman")->size[0], this->gameObjects.at("hooman")->size[1], element.second->position[0], element.second->position[1], element.second->size[0], element.second->size[1]);
                if (result) {
                    gravity.applyForce(&this->gameObjects.at("hooman")->position[0], &this->gameObjects.at("hooman")->position[1], -100.0f, this->playerInput, dt);
                    break;
                }
            }
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE) {
        if (Keys[GLFW_KEY_A]) {
            this->playerInput = m_LEFT;
            if (this->gameObjects.at("hooman")->currentAnim != "LEFT_WALK" && !Keys[GLFW_KEY_W] && !Keys[GLFW_KEY_S]) {
                this->initAnim(*this->gameObjects.at("hooman"), "LEFT_WALK");
            }
        }
        else if (Keys[GLFW_KEY_D]) {
            this->playerInput = m_RIGHT;
            if (this->gameObjects.at("hooman")->currentAnim != "RIGHT_WALK" && !Keys[GLFW_KEY_W] && !Keys[GLFW_KEY_S]) {
                this->initAnim(*this->gameObjects.at("hooman"), "RIGHT_WALK");
            }
        }
        if (Keys[GLFW_KEY_W]) {
            this->playerInput = m_UP;
            if (this->gameObjects.at("hooman")->currentAnim != "UP_WALK") {
                this->initAnim(*this->gameObjects.at("hooman"), "UP_WALK");
            }
        }
        else if (Keys[GLFW_KEY_S]) {
            this->playerInput = m_DOWN;
            if (this->gameObjects.at("hooman")->currentAnim != "DOWN_WALK") {
                this->initAnim(*this->gameObjects.at("hooman"), "DOWN_WALK");
            }
        }

        if (!Keys[GLFW_KEY_A] && !Keys[GLFW_KEY_D] && !Keys[GLFW_KEY_W] && !Keys[GLFW_KEY_S]) {
            this->playerInput = NONE;
            if (this->gameObjects.at("hooman")->currentAnim != "NONE") {
                this->setSprite(*this->gameObjects.at("hooman"), this->gameObjects.at("hooman")->currentAnim, 1);
            }
        }
    }
}

void Game::Render()
{
    for (std::pair<std::string, Object*> element : this->gameObjects) {
        if (element.second->visible) {
            if (element.second->repeatTexture) {
                for (int pos1 = element.second->position[0]; pos1 < element.second->position[0] + element.second->size[0]; pos1 += element.second->textureSize[0]) {
                    for (int pos2 = element.second->position[1]; pos2 < element.second->position[1] + element.second->size[1]; pos2 += element.second->textureSize[1]) {
                        Renderer->DrawSprite(ResourceManager::GetTexture(element.second->currentSprite), glm::vec2(pos1, pos2), glm::vec2(element.second->textureSize[0], element.second->textureSize[1]), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
                    }
                }
            }
            else {
                Renderer->DrawSprite(ResourceManager::GetTexture(element.second->currentSprite), glm::vec2(element.second->position[0], element.second->position[1]), glm::vec2(element.second->size[0], element.second->size[1]), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            }
        }
    }
}

void Game::moveCamera(float followX, float followY) {
    if (followX > this->screenRight - (this->Width / 3.5f)) {
        this->screenRight = followX + (this->Width / 3.5f);
        this->screenLeft = this->screenRight - this->Width;
    }
    else if (followX < this->screenLeft + (this->Width / 6.0f)) {
        this->screenLeft = followX - (this->Width / 6.0f);
        this->screenRight = this->screenLeft + this->Width;
    }
    if (followY < this->screenTop + (this->Height / 6.0f)) {
        this->screenTop = followY - (this->Height / 6.0f);
        this->screenDown = this->screenTop + this->Height;
    }
    else if (followY > this->screenDown - (this->Height / 4.0f)) {
        this->screenDown = followY + (this->Height / 4.0f);
        this->screenTop = this->screenDown - this->Height;
    }

    this->projection = glm::ortho(this->screenLeft, this->screenRight,
        this->screenDown, this->screenTop, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", this->projection);
}

void Game::loadTexture(std::map<std::string, std::string> sprites, animVars animationVars, std::string animName, Object& obj) {
    std::vector<std::string> animSprites;
    std::map<std::string, std::string>::iterator it;

    for (it = sprites.begin(); it != sprites.end(); it++) {
        ResourceManager::LoadTexture(const_cast<char*>(it->second.c_str()), true, it->first);
        animSprites.push_back(it->first);
    }
    
    obj.includeAnim(animName, animSprites, animationVars);
}

void Game::initAnim(Object& obj, std::string animation) {
    obj.animationVars.at(animation).initTime = time_point_cast<milliseconds>(system_clock::now());
    obj.animationVars.at(animation).currentFrame = 0;
    obj.animationVars.at(animation).previousFrame = 0;
    obj.currentSprite = obj.animations.at(animation).at(0);
    obj.currentAnim = animation;
}

void Game::runAnim(Object& obj, std::string animation) {
    time_point<system_clock, milliseconds> currentTime = time_point_cast<milliseconds>(system_clock::now());

    if (currentTime - obj.animationVars.at(animation).initTime > obj.animationVars.at(animation).animTime) {
        if (obj.animationVars.at(animation).currentFrame < obj.animations.at(animation).size()-1 && obj.animationVars.at(animation).currentFrame >= obj.animationVars.at(animation).previousFrame) {
            obj.animationVars.at(animation).initTime = currentTime;
            obj.animationVars.at(animation).previousFrame = obj.animationVars.at(animation).currentFrame;
            obj.animationVars.at(animation).currentFrame++;
            obj.currentSprite = obj.animations.at(animation).at(obj.animationVars.at(animation).currentFrame);
        }
        
        else if ((obj.animationVars.at(animation).currentFrame == obj.animations.at(animation).size()-1 && obj.animationVars.at(animation).bounce == true) || (obj.animationVars.at(animation).currentFrame > 0 && obj.animationVars.at(animation).currentFrame < obj.animationVars.at(animation).previousFrame)) {
            obj.animationVars.at(animation).initTime = currentTime;
            obj.animationVars.at(animation).previousFrame = obj.animationVars.at(animation).currentFrame;
            obj.animationVars.at(animation).currentFrame--;
            obj.currentSprite = obj.animations.at(animation).at(obj.animationVars.at(animation).currentFrame);
        }

        if ((obj.animationVars.at(animation).currentFrame == obj.animations.at(animation).size() && obj.animationVars.at(animation).bounce == false) || (obj.animationVars.at(animation).currentFrame == 0 && obj.animationVars.at(animation).currentFrame < obj.animationVars.at(animation).previousFrame) && obj.animationVars.at(animation).bounce == true) {
            this->initAnim(obj, animation);
        }
    }
}

void Game::setSprite(Object& obj, std::string animation, int frame) {
    obj.currentSprite = obj.animations.at(animation).at(frame);
    obj.currentAnim = "NONE";
}

void Game::readGameObjects(){
    std::ifstream gameObjectsFile("gameObjects/gameObjects.txt");

    std::vector<std::string> input;

    std::string data;

    if (gameObjectsFile.is_open()) {
        while (gameObjectsFile) {
            gameObjectsFile >> data;

            if (data != "#") {
                
                input.push_back(data);
            }

            else if (data == "#" && input.size() > 0) {
                this->gameObjects.insert(std::pair<std::string, Object*>(input.at(0), new Object(std::stof(input.at(2)), std::stof(input.at(3)), std::stof(input.at(4)), std::stof(input.at(5)), std::stof(input.at(6)), std::stof(input.at(7)), input.at(8) == "true", input.at(9) == "true", input.at(10) == "true")));
                if (input.at(1) == "Interactible") {
                    this->gameInteractibles.insert(std::pair<std::string, Interactible*>(input.at(0), new Interactible(this->gameObjects.at(input.at(0)))));
                }
                input.clear();
            }
        }
    }
}

void Game::readGameObjectsTextures() {
    std::ifstream gameObjectsFile("gameObjects/gameObjectsTextures.txt");
    std::vector<std::string> input;
    std::string data;
    std::string previousData = "";
    std::map<std::string, std::string> textureSprites;
    std::string mapPosKey;
    animVars hoomanAnimVars;

    if (gameObjectsFile.is_open()) {
        while (gameObjectsFile) {
            gameObjectsFile >> data;

            if (previousData == "*") {
                mapPosKey = data;
            }
            else if (previousData == "&") {
                textureSprites.insert(std::pair<std::string, std::string>(mapPosKey, data));
            }
            else if (data == "#" && input.size() > 0) {
                hoomanAnimVars.animTime = milliseconds(std::stoi(input.at(2)));
                hoomanAnimVars.bounce = (input.at(3) == "true");
                this->loadTexture(textureSprites, hoomanAnimVars, input.at(1), *this->gameObjects.at(input.at(0)));

                if (this->gameObjects.at(input.at(0))->currentAnim == "NONE") {
                    this->gameObjects.at(input.at(0))->currentAnim = input.at(1);
                }

                textureSprites.clear();
                input.clear();
            }
            else if (data != "*" && data != "&") {
                input.push_back(data);
            }

            previousData = data;
        }
    }
}

void Game::readGameScenarios() {
    std::ifstream scenariosFile("scenarios/scenario1.txt");
    std::vector<std::string> input;
    std::string data;
    std::string previousData = "";
    std::map<std::string, std::string> inputFunctions;
    std::string mapPosKey;

    if (scenariosFile.is_open()) {
        while (scenariosFile) {
            scenariosFile >> data;

            if (previousData == "*") {
            }
            else if (previousData == "&") {
            }
            else if (data == "#" && input.size() > 0) {
            }
            else if (data != "*" && data != "&") {
            }

            previousData = data;
        }
    }
}