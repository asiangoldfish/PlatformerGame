#pragma once

#include <Framework.h>

#include "Sprite.h"

class GameScene;

class Ship {
public:
    Ship();
    virtual ~Ship() = default;

    void update(float delta);

    FW::ref<FW::Sprite> getSprite() { return sprite; }

public: // Transformation
    void setPosition(float x, float y) { sprite->setPosition(x, y); }
    void setRotation(glm::vec3 rot) { sprite->setRotation(rot); }

private:
    friend GameScene;

    FW::ref<FW::Sprite> sprite;

private: // Player stats
    float health;
    float gold;
};