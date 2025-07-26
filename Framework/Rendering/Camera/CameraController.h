#pragma once

#include "pch.h"
#include "Shader.h"
#include "Camera.h"

namespace FW {
    class PerspectiveCamera;
    class OrthographicCamera;

    enum class CameraType
    {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC
    };

    /**
     * Controller for the Camera.
     *
     * While the Camera handles the technical details, the CameraController
     * handles user input and moving the camera. It provides functions to
     * manipulate the camera's properties.
     */
    class CameraController
    {
    public:
        CameraController() = default;
        explicit CameraController(CameraType type);
        virtual ~CameraController() = default;

        [[nodiscard]] ref<PerspectiveCamera>& getPerspectiveCamera()
        {
            return perspectiveCamera;
        }
        [[nodiscard]] ref<OrthographicCamera>& getOrthographicCamera()
        {
            return orthographicCamera;
        }

        /** Upload data to the GPU */
        void update(const ref<Shader>& shader);

        /**
         * Add rotation to the camera.
         *
         * @details Add rotation to the current camera's yaw and pitch. To set
         * the rotation's absolute value, use <u>setRotation()</u> instead.
         *
         * @param value Value to increment or decrement the current rotation by.
         */
        void addRotation(glm::vec2 value);

        /**
         * Set the camera absolute rotation.
         *
         * @details Set the camera's rotation by yaw and pitch. To increment or
         * decrement, instead use <u>addRotation()</u>
         *
         * @param value The values to set the new camera rotation by. The
         * x-value is yaw and y-value is pitch
         */
        void setRotation(glm::vec2 value);

        void moveForward(float value);
        void moveSideway(float value);
        void moveUp(float value);
        const glm::vec3 getPosition() const;

        void setPosition(glm::vec3 newPosition);
        void setPositionX(const float value);
        void setPositionY(const float value);
        void setPositionZ(const float value);

        [[nodiscard]] float getNearClip();
        void setNearClip(const float near);
        [[nodiscard]] float getFarClip();
        void setFarClip(const float far);

        const glm::vec3& getCameraOffset() const { return cameraOffset; }
        void setCameraOffset(glm::vec3 offset) { cameraOffset = offset; }

        bool getSpectatorMode() { return spectatorMode; }
        void setSpectatorMode(const bool enable) { spectatorMode = enable; }

        Camera* getSelectedCamera() { return selectedCamera; }
        /**
         * Move the entity.
         *
         * This function is a shorthand for currentPosition + unitsToMoveBy. It
         * takes the current position and adds x, y and z units to move by.
         *
         * @param moveBy How much to move by
         */
        void addMovement(glm::vec3 moveBy);

    private:
        Camera* selectedCamera = nullptr;
        ref<PerspectiveCamera> perspectiveCamera;
        ref<OrthographicCamera> orthographicCamera;

        /** Determines which camera to possess and initializing the correct data
         * accordingly */
        CameraType cameraType;
        bool degreesDirection = true;
        float degrees = 0.0f;
        float cameraDistance = 20.0f;
        float movementSpeed = 0.1f;
        float rotationSpeed = 0.1f;
        glm::vec3 worldCenter{ 0.0f };

        /**
         * How far away from the possessed entity should the camera be
         */
        glm::vec3 cameraOffset{ 0.0f };

        bool spectatorMode = false;
    };
}
