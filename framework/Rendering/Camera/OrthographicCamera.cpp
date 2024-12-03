#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace FW {
    OrthographicCamera::OrthographicCamera(const OrthographicCamera& other)
      : Camera(other)
    {
        frustum = other.frustum;
    }

    OrthographicCamera::OrthographicCamera(
      const OrthographicCamera::Frustum& frustum,
      const glm::vec3& position)
    {
        this->frustum = frustum;
        // Use the frustrum to construct the projection matrix
        projectionMatrix =
          glm::ortho(frustum.left, frustum.right, frustum.bottom, frustum.top);
    }

    void OrthographicCamera::update(const ref<Shader>& shader) {
        shader->bind();
        shader->setMat4("u_projection", getProjectionMatrix());
        shader->setMat4("u_view", getViewMatrix());
        shader->setFloat3("u_cameraPosition", getPosition());
        shader->setFloat("u_nearClip", frustum.near);
        shader->setFloat("u_farClip", frustum.far);
    }

    void OrthographicCamera::computeViewMatrix()
    {
        viewMatrix = glm::lookAt(position, { 0, 0, 1 }, { 0, 1, 0 });
    }
    void OrthographicCamera::computeProjectionMatrix() {
        projectionMatrix =
          glm::ortho(frustum.left, frustum.right, frustum.bottom, frustum.top);
    }
}
