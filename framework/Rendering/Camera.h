/**
 * A major part of the camera movement algorithm is from ChatGPT. Link to the
 * algorithm is below:
 * https://chat.openai.com/share/ceee7dc8-6d39-4e89-9c91-40fd646e58e0.
 * Code that uses the source will mention this.
 */

#pragma once

#include <glm/glm.hpp>

namespace Framework {

    class Camera
    {
    public:
        Camera() = default;
        virtual ~Camera() = default;

        /**
         * Initialize camera
         * The view matrix will be constructed. This should be called by the
         * user, as the camera may be orthographic or perspective. These two
         * projections have different view matrices.
         */
        void initializeCamera() { computeViewMatrix(); }

        inline glm::mat4 getViewMatrix() const { return viewMatrix; }
        inline glm::mat4 getProjectionMatrix() const
        {
            return projectionMatrix;
        }

        // -------------
        // Camera Movement
        // -------------
        void moveForward(const float speed);
        void moveSideway(const float speed);
        void moveUp(const float speed);
        void rotate(const glm::vec2& rotation);

        // -------------
        // Camera Rotation
        // -------------
        inline float getYaw() const { return yaw; }
        inline void setYaw(const float yaw) { this->yaw = yaw; }

        inline float getPitch() const { return pitch; }
        inline void setPitch(const float pitch) { this->pitch = pitch; }

        // -------------
        // Camera Position
        // -------------
        inline const glm::vec3& getPosition() { return position; }
        inline void setPosition(const glm::vec3& pos)
        {
            position = pos;
            computeViewMatrix();
        }

        // -------------
        // Camera Position
        // -------------
        inline float getNearClip() const { return nearClip; }
        inline void setNearClip(const float val) { nearClip = val; }
        inline float getFarClip() const { return farClip; }
        inline void setFarClip(const float val) { nearClip = val; }

        void setEnablePanning(bool enable) { enablePanning = enable; }

        // -------------
        // Camera angle
        // -------------
        void setLookAtCenter(const glm::vec3& center) { lookAtCenter = center; }

    private:
        /**
         * @brief Computes the camera's front vector and view matrix
         */
        virtual void computeViewMatrix() = 0;

    protected:
        float yaw = 0.0f;
        float pitch = 0.0f;
        glm::vec3 cameraFront = glm::vec3(0.0f);
        glm::mat4 viewMatrix = glm::mat4(1.0f);
        glm::vec3 cameraUp = { 0.0f, 1.0f, 0.0f };
        glm::mat4 projectionMatrix = glm::mat4(1.0f);
        glm::vec3 position = glm::vec3(0.0f);
        float nearClip = 0.0f, farClip = 0.0f;

        // When enablePanning is off, determine where the camera should look at
        glm::vec3 lookAtCenter = glm::vec3(0.0f);

        bool enablePanning = true;
    };

}
