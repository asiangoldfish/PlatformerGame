#include "CameraController.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "Camera.h"

// TODO Find some way so the camera does not have to know what the shader is
void CameraController::update(const ref<Shader>& shader) {
    selectedCamera->update(shader);
}

CameraController::CameraController(CameraType type)
  : cameraType(type) {
    /*
     * Based on the selected camera, instantiate the correct type.
     */
    switch (cameraType) {
        case CameraType::PERSPECTIVE:
            perspectiveCamera = createRef<PerspectiveCamera>();

            selectedCamera = perspectiveCamera.get();
            break;

        case CameraType::ORTHOGRAPHIC:
            orthographicCamera = createRef<OrthographicCamera>();
            selectedCamera = orthographicCamera.get();
            break;
    }
}

void CameraController::addRotation(glm::vec2 value) {
    selectedCamera->rotate(value);
}

void CameraController::setRotation(glm::vec2 value) {
    selectedCamera->setRotation(value);
}

void CameraController::moveSideway(float value) {
    selectedCamera->moveSideway(value * movementSpeed);
}

void CameraController::moveForward(float value) {
    selectedCamera->moveForward(value * movementSpeed);
}

void CameraController::moveUp(float value) {
    selectedCamera->moveUp(value * movementSpeed);
}

const glm::vec3 CameraController::getPosition() const {
    if (selectedCamera) {
        return selectedCamera->getPosition();
    }

    return glm::vec3{ 0.0f };
}

void CameraController::setPosition(glm::vec3 newPosition) {
    if (selectedCamera) {
        selectedCamera->setPosition(newPosition);
    }
}

void CameraController::setPositionX(const float value) {
    auto pos = selectedCamera->getPosition();
    setPosition(glm::vec3{ value, pos.y, pos.z });
}
void CameraController::setPositionY(const float value) {
    auto pos = selectedCamera->getPosition();
    setPosition(glm::vec3{ pos.x, value, pos.z });
}
void CameraController::setPositionZ(const float value) {
    auto pos = selectedCamera->getPosition();
    setPosition(glm::vec3{ pos.x, pos.y, value });
}

float CameraController::getNearClip() {
    if (selectedCamera == (Camera*)perspectiveCamera.get()) {
        return perspectiveCamera->getFrustum().nearClip;
    } else {
        return orthographicCamera->getFrustum().near;
    }
}

void CameraController::setNearClip(const float near) {
    if (selectedCamera == (Camera*)perspectiveCamera.get()) {
        perspectiveCamera->getFrustum().nearClip = near;
    } else {
        orthographicCamera->getFrustum().near = near;
    }
}

float CameraController::getFarClip() {
    if (selectedCamera == (Camera*)perspectiveCamera.get()) {
        return perspectiveCamera->getFrustum().farClip;
    } else {
        return orthographicCamera->getFrustum().far;
    }
}

void CameraController::setFarClip(const float far) {
    if (selectedCamera == (Camera*)perspectiveCamera.get()) {
        perspectiveCamera->getFrustum().farClip = far;
    } else {
        orthographicCamera->getFrustum().far = far;
    }
}
void CameraController::addMovement(glm::vec3 moveBy) {
    selectedCamera->setPosition(selectedCamera->getPosition() +
                                glm::vec3(moveBy));
}
