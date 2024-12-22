#pragma once

// External libraries
#include "glm/glm.hpp"

// Framework
#include "pch.h"
#include "Camera.h"

namespace FW {
    class PerspectiveCamera : public Camera {
    public:
        struct Frustum {
            float angle, width, height, nearClip, farClip;
            Frustum() = default;
            Frustum(float angle,
                    float width,
                    float height,
                    float nearClip,
                    float farClip)
              : angle(angle)
              , width(width)
              , height(height)
              , nearClip(nearClip)
              , farClip(farClip) {}

            glm::vec2 getSize() { return glm::vec2{ width, height }; }

            /**
             * Update the frustum's width and height.
             *
             * This must match the window resolution (not monitor).
             */
            void setSize(glm::vec2 size) {
                this->width = size.x;
                this->height = size.y;
            }
        };

    public:
        /** Perspective camera constructor */
        PerspectiveCamera(
          const Frustum& frustum = { 45.0f, 1920.0f, 1080.0f, 0.1f, 100.0f },
          const glm::vec3& position = glm::vec3(0.0f));

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

        Frustum& getFrustum() { return frustum; }

        void setLookAt(const glm::vec3& vec) { this->lookAt = vec; }
        void setUpVector(const glm::vec3& vec) { this->upVector = vec; }

        void computeProjectionMatrix() override;
        void update(const ref<Shader>& shader) override;

        void updateViewportSize(const glm::vec2& size) override;

    protected:
        void computeViewMatrix() override;

    protected:
        glm::vec3 lookAt, upVector;
        Frustum frustum;
    };
} // Framework
