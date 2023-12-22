#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

Framework::OrthographicCamera::OrthographicCamera(
        const Framework::OrthographicCamera &other)
        : Camera(other) {
    frustrum = other.frustrum;
}

Framework::OrthographicCamera::OrthographicCamera(
        const Framework::OrthographicCamera::Frustrum &frustrum,
        const glm::vec3 &position) {

    // Use the frustrum to construct the projection matrix
    projectionMatrix = glm::ortho(
        frustrum.left, frustrum.right, frustrum.bottom, frustrum.top
    );
}

void Framework::OrthographicCamera::computeViewMatrix() {
    viewMatrix = glm::lookAt(position, {0, 0, 1}, {0, 1, 0});
}
