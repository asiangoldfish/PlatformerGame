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
     * @details The player controller does not manage the possessed entity's
     * lifetime. Memory management must be performed by the user.
     */
    class PlayerController
    {
    public:
        /**
         * Construct a player controller with default parameters.
         */
        PlayerController() = default;

        /**
         * Construct the player controller and immediately possess an entity.
         * @param entity The entity to possess.
         */
        PlayerController(ref<Entity>& entity);

        /**
         * Default deconstructor for the Player Controller class
         */
        virtual ~PlayerController() = default;

        /**
         * Possess an entity.
         * @details The player controller does not manage the entity. Memory
         * management must be performed by the user.
         * @param entity The entity to possess.
         */
        void possessEntity(ref<Entity>& entity);

        /**
         * Get a reference to the possessed entity
         * @return The possessed entity
         */
        ref<Entity> getPossessedEntity() { return possessedEntity; }

        /**
         * Add movement to the entity.
         * @details Add the passed parameters to the current x, y and z
         * position.
         * @param moveBy How much to move the entity by.
         */
        void addMovement(glm::vec3 moveBy);

        /**
         * Add rotation to the entity.
         * @details Add the passed parameters to the current yaw, pitch and
         * roll.
         * @param yawBy How much to add to the current yaw.
         * @param pitchBy How much to add to the current pitch.
         * @param rollBy How much to add to the current rollBy.
         */
        void addRotation(float yawBy, float pitchBy, float rollBy);

    private:
        ref<Entity> possessedEntity;
        float yaw;
        float pitch;
        float roll;
    };
}
