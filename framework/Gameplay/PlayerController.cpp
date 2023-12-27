#include "PlayerController.h"
#include "Entity.h"

namespace FW {
    void PlayerController::addMovement(glm::vec3 moveBy)
    {
        possessedEntity->setPosition(possessedEntity->getPosition() +
                                     glm::vec3(moveBy));
        possessedEntity->recalculateModelMatrix();
    }

    void PlayerController::addRotation(float yawBy, float pitchBy, float rollBy)
    {
        yaw += yawBy;
        pitch += pitchBy;
        roll += rollBy;
        possessedEntity->recalculateModelMatrix();
    }

    PlayerController::PlayerController(scope<Entity>& entity)
    {
        possessedEntity = std::move(entity);
    }

    void PlayerController::possessEntity(scope<Entity>& entity)
    {
        possessedEntity = std::move(entity);
    }
} // FW
