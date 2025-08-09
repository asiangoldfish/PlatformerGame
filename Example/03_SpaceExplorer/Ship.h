#pragma once

#include <Framework.h>

#include "Sprite.h"

class GameScene;

class Ship {
public:
    Ship();
    virtual ~Ship() = default;

    void update(float delta);

    FW::ref<Sprite> getSprite() { return sprite; }

public: // Transformation
    void setPosition(float x, float y) { sprite->setPosition(x, y); }
    void setRotation(glm::vec3 rot) { sprite->setRotation(rot); }

private:
    friend GameScene;

    FW::ref<Sprite> sprite;
    FW::ref<FW::OrthographicCamera> camera;

private: // Player stats
    float health;
    float gold;
};