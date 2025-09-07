#pragma once

#include "Ship.h"
#include "ECS_Systems.h"

/**
 * Manage ship target selection.
 */
class SelectionSystem : public FW::BaseSystem {
public:
    SelectionSystem() = default;
    virtual ~SelectionSystem() = default;

    virtual void update(float delta) override;

    void onHovering();
    void onClicking();

    void setEnemiesRoot(FW::ref<ShipCollection> root) { enemiesRoot = root; }

    void setPlayerShip(FW::ref<Ship> ship) { playerShip = ship; }

    void setCamera(FW::ref<FW::Camera> camera) { this->camera = camera; }

private:
    std::weak_ptr<ShipCollection> enemiesRoot;
    std::weak_ptr<Ship> playerShip;
    std::weak_ptr<FW::Camera> camera;
};