#pragma once

// External libraries
#include "glm/glm.hpp"

// Framework
#include "pch.h"
#include "Camera.h"

namespace Framework {
    class PerspectiveCamera : public Camera {
        // Camera(const glm::vec3& position, float yaw, float pitch, float nearClip, float farClip);
    public:
        struct Frustum
        {
            float angle, width, height, nearClip, farClip;
            Frustum() = default;
            Frustum(
                float angle,
                float width,
                float height,
                float nearClip,
                float farClip)
                : angle(angle)
                , width(width)
                , height(height)
                , nearClip(nearClip)
                , farClip(farClip) {}
        };

    public:
        /** Perspective camera constructor */
        PerspectiveCamera(
            const Frustum& frustrum = { 45.0f, 1280.0f, 720.0f, 0.1f, 100.0f },
            const glm::vec3& position = glm::vec3(0.0f)
        );

        virtual ~PerspectiveCamera() = default;

        /**
         * Copy constructor
         * We copy all the properties of camera to create a new instance with
         * the same properties. The new instance is an entirely separate object.
         *
         * @param camera Camera to construct properties from.
         */
        PerspectiveCamera(const PerspectiveCamera& camera);

        void setFrustum(const Frustum& frust) {
            this->frustum = frust;
            computeProjectionMatrix();
        }

        Frustum getFrustum() { return frustum; }

        void setLookAt(const glm::vec3& vec) {
            this->lookAt = vec;
        }
        void setUpVector(const glm::vec3& vec) {
            this->upVector = vec;
        }

        void computeProjectionMatrix();
        void update(Shader& shader) override;

    protected:
        virtual void computeViewMatrix() override;

    protected:
        glm::vec3 lookAt, upVector;
        Frustum frustum;
    };
} // Framework
