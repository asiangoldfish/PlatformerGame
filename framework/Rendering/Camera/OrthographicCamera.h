#pragma once

// C++ libraries
#include <array>

// External libraries
#include "glm/fwd.hpp"

// Framework
#include "Camera.h"

namespace Framework
{
    class OrthographicCamera : public Camera {
    public:
        struct Frustum
        {
            float left, right, bottom, top, near, far;
        };

    public:
        OrthographicCamera(const OrthographicCamera& other);
        OrthographicCamera(
            const Frustum& frustum = {
                    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f
            },
            const glm::vec3& position = glm::vec3(0.0f)
        );

        void update(Shader& shader) override {};

        void setFrustum(const Frustum& frust) {
            this->frustum = frust;
        }
        [[nodiscard]] Frustum& getFrustum() { return frustum; }

        void updateViewportSize(glm::vec2 size) override {}

    private:
        void computeViewMatrix() override;

    private:
        Frustum frustum;
    };
}
