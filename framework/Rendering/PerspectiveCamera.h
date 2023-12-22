#pragma once

// External libraries
#include <glm/glm.hpp>

// Framework
#include "pch.h"
#include "Camera.h"

namespace Framework {
    class PerspectiveCamera : public Camera {
        // Camera(const glm::vec3& position, float yaw, float pitch, float nearClip, float farClip);
    public:
        struct Frustrum {
            float angle, width, height, nearClip, farClip;
            Frustrum() = default;
            Frustrum(
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
            const Frustrum& frustrum = { 45.0f, -1.0f, 1.0f, -1.0f, 1.0f },
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

        void setFrustrum(const Frustrum& frust) {
            this->frustrum = frust;
            computeProjectionMatrix();
        }

        Frustrum getFrustrum() { return frustrum; }

        void setLookAt(const glm::vec3& vec) {
            this->lookAt = vec;
        }
        void setUpVector(const glm::vec3& vec) {
            this->upVector = vec;
        }

        void computeProjectionMatrix();

    protected:
        virtual void computeViewMatrix() override;

    protected:
        glm::vec3 lookAt, upVector;
        Frustrum frustrum;
    };
} // Framework
