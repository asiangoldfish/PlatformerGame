// C++ library
#include <cmath>
#include <iostream>

// External libraries
#include "glm/gtc/matrix_transform.hpp"

// Framework
#include "PerspectiveCamera.h"

namespace FW {
    PerspectiveCamera::PerspectiveCamera(
      const PerspectiveCamera::Frustum& frustum,
      const glm::vec3& position)
      : frustum(frustum)
    {
        // Use the frustrum to construct the projection matrix
        computeProjectionMatrix();

        this->position = position;

        INFO("PerspectiveCamera initialized");
    }

    PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera& camera)
      : Camera(camera)
    {
        lookAt = camera.lookAt;
        upVector = camera.upVector;
        frustum = camera.frustum;
    }

    void PerspectiveCamera::computeViewMatrix()
    {
        if (enablePanning) {
            glm::vec3 front(0.0f);

            // From ChatGPT:
            // https://chat.openai.com/share/ceee7dc8-6d39-4e89-9c91-40fd646e58e0
            front.x = std::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
            front.y = std::sin(glm::radians(pitch));
            front.z = std::sin(glm::radians(yaw)) * cos(glm::radians(pitch));
            cameraFront = glm::normalize(front);

            viewMatrix =
              glm::lookAt(position, position + cameraFront, cameraUp);
        } else {
            viewMatrix = glm::lookAt(position, lookAtCenter, cameraUp);
        }
    }

    void PerspectiveCamera::computeProjectionMatrix()
    {
        projectionMatrix =
          glm::perspective(glm::radians(frustum.angle),
                           (float)frustum.width / (float)frustum.height,
                           frustum.nearClip,
                           frustum.farClip);
    }

    void PerspectiveCamera::update(const ref<Shader>& shader)
    {
        shader->bind();
        shader->setMat4("u_projection", getProjectionMatrix());
        shader->setMat4("u_view", getViewMatrix());
        shader->setFloat3("u_cameraPosition", getPosition());
    }
    void PerspectiveCamera::updateViewportSize(glm::vec2 size)
    {
        // Match the camera frustum's width and height to the new window size
        FW::PerspectiveCamera::Frustum oldFrustum = frustum;

        oldFrustum.width = static_cast<float>(size.x);
        oldFrustum.height = static_cast<float>(size.y);
        setFrustum(oldFrustum);

        // We must compute the projection matrix again for the change to take
        // effect.
        computeProjectionMatrix();
    }
} // Framework
