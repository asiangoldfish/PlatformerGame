#pragma once

#include "pch.h"
#include "Core.h"
#include "Shader.h"

namespace Framework {
    class PerspectiveCamera;
    class OrthographicCamera;
    class Camera;

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

        [[nodiscard]] ref<PerspectiveCamera> getPerspectiveCamera() const
        {
            return perspectiveCamera;
        }
        [[nodiscard]] ref<OrthographicCamera> getOrthographicCamera() const
        {
            return orthographicCamera;
        }

        /** Upload data to the GPU */
        void update(Shader& shader);

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
        void setPosition(glm::vec3 newPosition);

        /**
         * Get the camera movement's speed.
         */
        float getCameraSpeed() const { return cameraSpeed; }

        /**
         * Set the camera movement's speed.
         * @param speed New camera speed.
         */
        void setCameraSpeed(const float speed) { cameraSpeed = speed; }

        void setNearClip(const float val);
        void setFarClip(const float val);

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
        float cameraSpeed = 0.1f;
        glm::vec3 worldCenter{ 0.0f };
    };
}
