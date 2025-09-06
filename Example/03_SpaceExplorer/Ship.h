#pragma once

#include <Framework.h>

#include "Sprite.h"
#include "BaseScene.h"

#include "TargetSelector.h"
#include "Stats.h"

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

    void setIsTargeted(const bool b);
    bool getIsTargeted() { return isTargeted; }

    void setTargetShip(FW::ref<Ship> targetShip);

    FW::ref<Ship> getTargetShip() { return targetShip; }

    /**
     * Set the selection state.
     */
    void setTargetedState(const TargetSelectionState type) {
        if (targetSelectorScene) {
            targetSelectorScene->setSelectionState(type);

            if (type == TargetSelectionState::INACTIVE) {
                removeChild(targetSelectorScene);
            } else {
                addChild(targetSelectorScene);
            }
        }
    }

    TargetSelectionState getTargetedState() {
        if (targetSelectorScene) {
            return targetSelectorScene->getSelectionState();
        }

        return TargetSelectionState::INACTIVE;
    }

public: // Combat related methods

    /**
     * Take damage. If enough damage is dealt, mark this ship as dead.
     */
    void takeDamage(float incomingDamage);

private:
    friend GameScene;

    bool isTargeted = false;

    bool isDead = false;

protected: // Player stats
    float health;
    float gold;
    FW::ref<FW::Camera> camera;

    float fireMaxCooldown = 1.0f;
    float fireCurrentCooldown = 0.0f;

    float randomSpreadRadius = glm::radians(0.0f);

    float speed = 100.0f;

    /**
     * Determines the colour of the targeted ring when player selects the ship
     */
    // We need a reference to this, as we would remove it from children if it
    // should no longer be rendered.
    FW::ref<TargetSelector> targetSelectorScene;

    // The ship will rotate and point toward the target, if a target is locked.
    FW::ref<Ship> targetShip;

    /** The ship will shoot the target if the target is within range. */
    float weaponRange = 400.0f;

public:
    VitalStats vitalStats;
    CombatStats combatStats;

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

    bool isDead = false;

    /** Once reaching the target, the bullet dies*/
    FW::ref<Ship> targetShip;
    /**
     * If the bullet is within d distance from the target destination, register
     * it as a hit.
     * */
    float collisionTolerance = 50.0f;

    float damage = 0.0f;
};

class PlayerShip : public Ship {
public:
    PlayerShip() = default;
    PlayerShip(FW::ref<FW::Camera> camera,
               FW::ref<ProjectileRoot> projectileRoot = nullptr);

    virtual void update(float delta) override;
};

class EnemyShip : public Ship {
public:
    EnemyShip() = default;
    EnemyShip(FW::ref<FW::Camera> camera,
              FW::ref<ProjectileRoot> projectileRoot = nullptr);

    virtual void update(float delta) override;
};
