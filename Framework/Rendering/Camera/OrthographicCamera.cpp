#include "OrthographicCamera.h"
#include "glm/gtc/matrix_transform.hpp"

namespace FW {
    OrthographicCamera::OrthographicCamera(const OrthographicCamera& other)
      : Camera(other)
    {
        frustum = other.frustum;
        computeProjectionMatrix();
    }

    OrthographicCamera::OrthographicCamera(
      const OrthographicCamera::Frustum& frustum,
      const glm::vec3& position)
    {
        this->frustum = frustum;
        // Use the frustrum to construct the projection matrix
        projectionMatrix =
          glm::ortho(frustum.left, frustum.right, frustum.bottom, frustum.top);
        
        computeProjectionMatrix();
        this->position = position;
    }

    void OrthographicCamera::update(const ref<Shader>& shader) {
        shader->bind();
        shader->setMat4("u_projection", getProjectionMatrix());
        shader->setMat4("u_view", getViewMatrix());
        shader->setFloat3("u_cameraPosition", getPosition());
        shader->setFloat("u_nearClip", frustum.near);
        shader->setFloat("u_farClip", frustum.far);
    }
    
    
    void OrthographicCamera::update(const Shader* shader) {
        shader->bind();
        shader->setMat4("u_projection", getProjectionMatrix());
        shader->setMat4("u_view", getViewMatrix());
        shader->setFloat3("u_cameraPosition", getPosition());
        shader->setFloat("u_nearClip", frustum.near);
        shader->setFloat("u_farClip", frustum.far);
    }

    void OrthographicCamera::setCentraliseScreenCoordinates(bool b) {
        centraliseScreenCoordinates = b;
        computeProjectionMatrix();
    }
    void OrthographicCamera::computeViewMatrix() {
        viewMatrix = glm::lookAt(position, { position.x, position.y, -1.0f }, { 0.0, 1, 0.0 });
    }

    void OrthographicCamera::updateViewportSize(const glm::vec2& size) {
        frustum.right = size.x;
        frustum.top = size.y;
        glViewport(0, 0, size.x, size.y);

        // We must compute the projection matrix again for the change to take
        // effect.
        computeProjectionMatrix();
    }

    void OrthographicCamera::setCameraSize(float x, float y) {
        glm::vec2 newSize{x, y};
        updateViewportSize(newSize);
    }

    void OrthographicCamera::computeProjectionMatrix() {
        float left = centraliseScreenCoordinates ? -frustum.right * 0.5f : 0.0f;
        float right =
          centraliseScreenCoordinates ? frustum.right * 0.5f : frustum.right;
        float bottom = centraliseScreenCoordinates ? -frustum.top * 0.5f : 0.0f;
        float top =
          centraliseScreenCoordinates ? frustum.top * 0.5f : frustum.top;

        projectionMatrix =
          glm::ortho(left, right, bottom, top, frustum.near, frustum.far);
    }
}
