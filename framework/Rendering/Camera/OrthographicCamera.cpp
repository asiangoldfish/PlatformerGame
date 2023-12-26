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

        // Use the frustrum to construct the projection matrix
        projectionMatrix =
          glm::ortho(frustum.left, frustum.right, frustum.bottom, frustum.top);
    }

    void OrthographicCamera::computeViewMatrix()
    {
        viewMatrix = glm::lookAt(position, { 0, 0, 1 }, { 0, 1, 0 });
    }
}
