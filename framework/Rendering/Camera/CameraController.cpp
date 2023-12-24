#include "CameraController.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"
#include "Camera.h"

namespace Framework {
    void CameraController::update(Shader& shader)
    {
        selectedCamera->update(shader);
    }

    CameraController::CameraController(CameraType type)
      : cameraType(type)
    {
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

    void CameraController::rotate(glm::vec2 value)
    {
        selectedCamera->rotate(value);
    }

    void CameraController::moveSideway(float value)
    {
        selectedCamera->moveSideway(value);
    }

    void CameraController::moveForward(float value)
    {
        selectedCamera->moveForward(value);
    }

    void CameraController::moveUp(float value)
    {
        selectedCamera->moveUp(value);
    }

    const glm::vec3 CameraController::getPosition() const
    {
        if (selectedCamera) {
            return selectedCamera->getPosition();
        }

        return glm::vec3{ 0.0f };
    }

    void CameraController::setPosition(glm::vec3 newPosition)
    {
        if (selectedCamera) {
            selectedCamera->setPosition(newPosition);
        }
    }

    float CameraController::getNearClip()
    {
        if (selectedCamera == (Camera*)perspectiveCamera.get()) {
            return perspectiveCamera->getFrustum().nearClip;
        } else {
            return orthographicCamera->getFrustum().near;
        }
    }

    void CameraController::setNearClip(const float near)
    {
        if (selectedCamera == (Camera*)perspectiveCamera.get()) {
            perspectiveCamera->getFrustum().nearClip = near;
        } else {
            orthographicCamera->getFrustum().near = near;
        }
    }

    float CameraController::getFarClip()
    {
        if (selectedCamera == (Camera*)perspectiveCamera.get()) {
            return perspectiveCamera->getFrustum().farClip;
        } else {
            return orthographicCamera->getFrustum().far;
        }
    }

    void CameraController::setFarClip(const float far)
    {
        if (selectedCamera == (Camera*)perspectiveCamera.get()) {
            perspectiveCamera->getFrustum().farClip = far;
        } else {
            orthographicCamera->getFrustum().far = far;
        }
    }
}
