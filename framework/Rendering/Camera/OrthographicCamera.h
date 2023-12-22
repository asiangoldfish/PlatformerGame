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
        struct Frustrum {
            float left, right, bottom, top, near, far;
        };

    public:
        OrthographicCamera(const OrthographicCamera& other);
        OrthographicCamera(
            const Frustrum& frustrum = {
                    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f
            },
            const glm::vec3& position = glm::vec3(0.0f)
        );

        void update(Shader& shader) override {};

        void setFrustrum(const Frustrum& frust) {
            this->frustrum = frust;
        }

    private:
        void computeViewMatrix() override;

    private:
        Frustrum frustrum;
    };
}
