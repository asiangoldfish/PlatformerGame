#pragma once

#include "pch.h"

#include <glm/glm.hpp>

namespace FW {
    class Entity;

    /**
     * Take input player input and translate to gameplay actions.
     *
     * @details The player controller handles inputs from peripherals and
     * translates to in-game actions.
     */
    class PlayerController
    {
    public:
        PlayerController() = default;
        PlayerController(scope<Entity>& entity);
        virtual ~PlayerController() = default;

        void possessEntity(scope<Entity>& entity);

        Entity* getPossessedEntity() { return possessedEntity.get(); }

        /**
         * Move the entity.
         *
         * This function is a shorthand for currentPosition + unitsToMoveBy. It
         * takes the current position and adds x, y and z units to move by.
         *
         * @param moveBy How much to move by
         */
        void addMovement(glm::vec3 moveBy);

        /**
         * Rotate by some degrees
         */
        void addRotation(float yawBy, float pitchBy, float rollBy);

    private:
        scope<Entity> possessedEntity;
        float yaw;
        float pitch;
        float roll;
    };
}
