#pragma once

// C++ libraries
#include <array>

// External libraries
#include "glm/fwd.hpp"

// Framework
#include "Camera.h"

class OrthographicCamera : public Camera {
public:
    struct Frustum {
        float left, right, bottom, top, near, far;
    };

public:
    OrthographicCamera(const OrthographicCamera& other);
    OrthographicCamera(
      const Frustum& frustum = { 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 10.0f },
      // Because all entities in 2d are positioned by default at Z=0.0,
      // we should move the camera a little backward
      const glm::vec3& position = glm::vec3(0.0f, 0.0f, -0.01f));

    void update(const ref<Shader>& shader) override;

    void setFrustum(const Frustum& frust) {
        this->frustum = frust;
        computeProjectionMatrix();
    }

    [[nodiscard]] Frustum& getFrustum() { return frustum; }

    void updateViewportSize(const glm::vec2& size) override;

    /**
     * Set the camera sensor size to be `x` and `y`.
     *
     * This value in pixels is usually the client window's size. It should
     * reflect the render's output's size.
     *
     * This method does not change the camera coordinates; it only impacts
     * what screen coordinate is visible.
     */
    void setCameraSize(float x, float y);

    void computeProjectionMatrix() override;

    /**
     * Centralise the screen coordinates.
     *
     * By default, (0, 0) is at the bottom left of the screen. If `true`,
     * then (0, 0) is in the middle of the screen.
     *
     * This function should be changed to
     */
    void setCentraliseScreenCoordinates(bool b);

private:
    void computeViewMatrix() override;
    bool centraliseScreenCoordinates = false;

private:
    Frustum frustum;
};
