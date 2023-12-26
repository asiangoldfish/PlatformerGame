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
     * Manage the input for the Camera.
     *
     * While the Camera handles the technical details, the CameraController
     * handles user input and moving the camera.
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
         * Rotate the camera.
         *
         * The values are set in world space.
         * @param value The values to set the new camera rotation by. The
         * x-value is horizontal rotation, while the y-value is vertical.
         */
        void rotate(glm::vec2 value);

        void moveForward(float value);
        void moveSideway(float value);
        void moveUp(float value);
        const glm::vec3 getPosition() const;
        void setPosition(glm::vec3 newPosition);

        [[nodiscard]] float getNearClip();
        void setNearClip(const float near);
        [[nodiscard]] float getFarClip();
        void setFarClip(const float far);

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
    };
}
