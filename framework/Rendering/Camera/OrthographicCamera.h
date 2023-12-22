#ifndef ORTHOGRAPHIC_CAMERA_H
#define ORTHOGRAPHIC_CAMERA_H

#include "Camera.h"
#include "glm/fwd.hpp"
#include <array>

namespace Framework
{
    class OrthographicCamera : public Camera {
    public:
        struct Frustrum {
            float left, right, bottom, top, near, far;
        };

    public:
        OrthographicCamera() = default;
        OrthographicCamera(const OrthographicCamera& other);
        OrthographicCamera(
            const Frustrum& frustrum = {
                    -1.0f, 1.0f, -1.0f, 1.0f, 1.0f, -1.0f
            },
            const glm::vec3& position = glm::vec3(0.0f)
        );

        void setFrustrum(const Frustrum& frust) {
            this->frustrum = frust;
        }

    private:
        void computeViewMatrix() override;

    private:
        Frustrum frustrum;
    };
}


#endif // ORTHOGRAPHIC_CAMERA_H
