#pragma once

#include <Framework.h>

#include "Sprite.h"
#include "BaseScene.h"

class GameScene;

class Ship : public FW::SceneNode {
public:
    Ship() = default;
    Ship(FW::ref<FW::Camera> camera);
    virtual ~Ship() = default;

public: // Transformation
    void setPosition(float x, float y);
    glm::vec2 getPosition();
    void setRotation(glm::vec3 rot);

private:
    friend GameScene;

private: // Player stats
    float health;
    float gold;
};

class Bullet : public FW::SceneNode {
public:
    Bullet() = default;
    virtual ~Bullet() = default;

    virtual void update(float delta) override;
    void setPosition(float x, float y);
    void setPosition(glm::vec2 pos);
    glm::vec2 getPosition();
    void setRotation(glm::vec3 rot);

public:
    glm::vec2 velocity{ 0.0f };
    float maxTime = 0.5f;
    float time = 0.0f;
};

class ProjectileRoot : public FW::SceneNode {
public:
    ProjectileRoot() = default;
    virtual ~ProjectileRoot() = default;

    virtual void update(float delta) override;
};