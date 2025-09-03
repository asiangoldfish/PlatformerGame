#pragma once

#include "Framework.h"
#include "Ship.h"
#include "GameUI.h"

#include "ECS_Systems.h"

class GameScene : public FW::BaseScene {
public:
    GameScene() = default;
    virtual ~GameScene() = default;
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void keyCallback(int key, int scancode, int action, int mods);
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);

private:
    FW::ref<FW::OrthographicCamera> camera;
    FW::ref<Ship> playerShip;
    FW::ref<FW::SceneNode> backgroundNode;
    /** Simulates space objects to be far away in the background */
    float parallaxFactor = 0.2f;

    /** We put all bullets and stuff here, so they won't move with for example
     * the ship that shot the projectile. */
    FW::ref<ProjectileRoot> projectileRoot;

    // FW::ref<GameUI> gameUI;

    FW::RenderSystem renderSystem;
};
