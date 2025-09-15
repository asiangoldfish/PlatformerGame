#pragma once

#include "Ship.h"
#include "ECS_Systems.h"
#include "Debugging.h"
#include "systems/Systems.h"
#include "systems/Map.h"

class GameScene : public FW::BaseScene {
public:
    GameScene() = default;
    virtual ~GameScene() = default;
    virtual void init() override;
    virtual void cleanUp() override;
    virtual void update(float delta) override;

    void keyCallback(int key, int scancode, int action, int mods);
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);

    /** Invoked by the Application class. */
    void setDebugging(FW::ref<Debugging> d);

private:
    FW::ref<FW::OrthographicCamera> camera;
    FW::ref<PlayerShip> playerShip;
    FW::ref<FW::SceneNode> backgroundNode;
    /** Simulates space objects to be far away in the background */
    float parallaxFactor = 0.2f;

    /** We put all bullets and stuff here, so they won't move with for example
     * the ship that shot the projectile. */
    FW::ref<ProjectileRoot> projectileRoot;

    FW::ref<ShipCollection> enemyShips;

    // FW::ref<GameUI> gameUI;

public: // Systems
    FW::RenderSystem renderSystem;
    SelectionSystem selectionSystem;
    MapSystem mapSystem;

private: // Development
    FW::ref<Debugging> debugging;
};
