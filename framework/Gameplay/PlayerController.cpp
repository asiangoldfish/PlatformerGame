#include "PlayerController.h"
#include "Entity.h"

namespace FW {
    void PlayerController::addMovement(glm::vec3 moveBy)
    {
        // possessedEntity->setPosition(possessedEntity->getPosition() +
        //                              glm::vec3(moveBy));
        // possessedEntity->recalculateModelMatrix();
    }

    void PlayerController::addRotation(float yawBy, float pitchBy, float rollBy)
    {
        yaw += yawBy;
        pitch += pitchBy;
        roll += rollBy;
        // possessedEntity->recalculateModelMatrix();
    }

    PlayerController::PlayerController(ref<Entity>& entity)
    {
        possessedEntity = entity;
    }

    void PlayerController::possessEntity(ref<Entity>& entity)
    {
        possessedEntity = entity;
    }
} // FW
