#pragma once

#include <Framework.h>

#include "Sprite.h"
#include "BaseScene.h"


class GameScene;

class ProjectileRoot : public FW::SceneNode {
public:
    ProjectileRoot() = default;
    virtual ~ProjectileRoot() = default;

    virtual void update(float delta) override;
};

class Ship : public FW::SceneNode {
public:
    Ship() = default;
    Ship(FW::ref<FW::Camera> camera, FW::ref<ProjectileRoot> projectileRoot);
    virtual ~Ship() = default;

public: // Transformation
    void setPosition(float x, float y);
    void setPosition(float x, float y, float z);
    void setPosition(glm::vec2 pos);
    glm::vec2 getPosition();
    void setRotation(glm::vec3 rot);

    virtual void update(float delta) override;

    /**
     * Fire bullets.
     *
     * The bullets should move independently from the space ship, meaning the
     * space ship itself has no knowledge of the bullets. The user must
     * therefore pass a reference to the parent node.
     *
     * @param root the node to attach the bullets to
     */
    virtual void fireBullets(FW::ref<FW::SceneNode> root);

    void setZIndex(uint32_t z);

private:
    friend GameScene;

protected: // Player stats
    float health;
    float gold;
    FW::ref<FW::Camera> camera;

    float fireMaxCooldown = 0.075f;
    float fireCurrentCooldown = 0.0f;

    float randomSpreadRadius = glm::radians(15.0f);


protected:
    FW::ref<ProjectileRoot> projectileRoot;
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

class PlayerShip : public Ship {
public:
    PlayerShip() = default;
    PlayerShip(FW::ref<FW::Camera> camera, FW::ref<ProjectileRoot> projectileRoot = nullptr)
      : Ship(camera, projectileRoot) {}

    virtual void update(float delta) override;
};

class EnemyShip : public Ship {
public:
    EnemyShip() = default;
    EnemyShip(FW::ref<FW::Camera> camera, FW::ref<ProjectileRoot> projectileRoot = nullptr)
      : Ship(camera, projectileRoot) {}

    virtual void update(float delta) override;
};
