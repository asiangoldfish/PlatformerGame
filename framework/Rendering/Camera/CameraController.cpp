#include "CameraController.h"
#include "PerspectiveCamera.h"
#include "OrthographicCamera.h"

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
        switch(cameraType) {
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

    void CameraController::setPosition(glm::vec3 newPosition) {
        if(selectedCamera) {
            selectedCamera->setPosition(newPosition);
        }
    }
}
