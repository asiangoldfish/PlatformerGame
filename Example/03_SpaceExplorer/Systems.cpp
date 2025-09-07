#include "Systems.h"
#include "SpaceExplorerUtils.h"

void SelectionSystem::update(float delta) {}

void SelectionSystem::onHovering() {
    // When hovering over an enemy ship and it isn't hovered, then make a gray
    // selection outline
    auto cam = camera.lock();
    auto enemies = enemiesRoot.lock();

    if (cam && enemies) {
        for (auto& childNode : enemies->childNodes) {

            FW::ref<EnemyShip> enemyShip =
              std::dynamic_pointer_cast<EnemyShip>(childNode);

            if (enemyShip && !enemyShip->getIsTargeted()) {
                bool isInBounds = isMouseInsideEntityBounds(enemyShip, cam);

                // Only hover if it isn't already hovered
                if (isInBounds && enemyShip->getTargetedState() ==
                                    TargetSelectionState::INACTIVE) {
                    enemyShip->setTargetedState(TargetSelectionState::HOVERED);
                } else if (enemyShip->getTargetedState() ==
                             TargetSelectionState::HOVERED &&
                           !isInBounds) {
                    enemyShip->setTargetedState(TargetSelectionState::INACTIVE);
                }
            }
        }
    }
}

void SelectionSystem::onClicking() {
    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {

        auto player = playerShip.lock();
        auto cam = camera.lock();
        auto enemies = enemiesRoot.lock();

        if (!player || !cam || !enemies) {
            return;
        }

        // Deselect player targeted ship by clicking somewhere else than the
        // targeted ship.
        // Note that the player can still select themselves without
        // deselecting the target.
        FW::ref<Ship> targetShip = player->getTargetShip();
        if (targetShip && !isMouseInsideEntityBounds(targetShip, cam)) {
            targetShip->setIsTargeted(false);
            targetShip->setTargetedState(TargetSelectionState::INACTIVE);
            player->setTargetShip(nullptr);
        }

        // Ensure that if the player ship was just deselected, we won't
        // select it again. Also, target the first clicked ship only.
        for (auto& childNode : enemies->childNodes) {
            FW::ref<FW::Entity> enemyEntity = childNode->entity;

            if (enemyEntity && isMouseInsideEntityBounds(childNode, cam)) {
                FW::ref<Ship> enemyShip = std::dynamic_pointer_cast<Ship>(childNode);

                if (enemyShip) {
                    // If the player just deselected themselves, make sure
                    // we don't reselect the player again.
                    // Now, set the target
                    if (!enemyShip->getIsTargeted() && enemyShip != player) {
                        player->setTargetShip(enemyShip);
                        enemyShip->setTargetedState(TargetSelectionState::ACTIVE);
                    }
                    break;
                }
            }
        }
    }
}
