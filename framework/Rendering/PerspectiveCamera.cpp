#include "PerspectiveCamera.h"
#include <glm/gtc/matrix_transform.hpp>

#include <cmath>

#include <iostream>
namespace Framework {
    PerspectiveCamera::PerspectiveCamera(
            const PerspectiveCamera::Frustrum &frustrum,
            const glm::vec3 &position
            )
            : frustrum(frustrum)
    {
        // Use the frustrum to construct the projection matrix
        computeProjectionMatrix();

        this->position = position;
    }

    PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera &camera)
            : Camera(camera)
    {
        lookAt = camera.lookAt;
        upVector = camera.upVector;
        frustrum = camera.frustrum;
    }

    void PerspectiveCamera::computeViewMatrix() {
        if (enablePanning) {
            glm::vec3 front(0.0f);

            // From ChatGPT:
            // https://chat.openai.com/share/ceee7dc8-6d39-4e89-9c91-40fd646e58e0
            front.x = std::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = std::sin(glm::radians(pitch));
            front.z = std::sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);

            viewMatrix = glm::lookAt(position, position + cameraFront,
                                     cameraUp);
        } else {
            viewMatrix = glm::lookAt(position, lookAtCenter, cameraUp);
        }
    }

    void PerspectiveCamera::computeProjectionMatrix() {
        projectionMatrix = glm::perspective(
            glm::radians(frustrum.angle),
            frustrum.width/frustrum.height,
            frustrum.nearClip,
            frustrum.farClip
        );
    }
} // Framework
