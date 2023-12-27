#pragma once

#include <string>
#include <memory>

#include "Framework.h"

class Cube;
class Floor;

namespace FW {
    class Shader;
    class PerspectiveCamera;
    class TextureManager;
}

class PhysicsApp : public FW::GLFWApplication
{
public:
    /**
     * Constructor. Remember to call init() before using the application.
     * @param name Window class and application name
     * @param version Application version
     */
    PhysicsApp(const std::string& name,
               const std::string& version,
               glm::vec2 windowSize = glm::vec2(1024, 1024))
      : GLFWApplication(name, version, windowSize)
    {
    }

    virtual ~PhysicsApp();

    /** Initialize the application. Must be called before using it */
    bool init() override;
    /** Game loop. Runs until the application quits. */
    void run() override;

    // ---------
    // Camera and rendering
    // ---------
    // Camera
    [[nodiscard]] FW::ref<FW::CameraController>& getCameraController()
    {
        return cameraController;
    }

    [[nodiscard]] FW::ref<FW::Shader> getShader() { return shader; }

    // --------
    // Inputs
    // --------
    /** Called every frame */
    void keyboardInput();

    [[nodiscard]] bool getIsRightButtonPressed() const
    {
        return isRightButtonPressed;
    }
    void setIsRightButtonPressed(const bool isPressed)
    {
        isRightButtonPressed = isPressed;
    }

    FW::Timer& getTimer() { return timer; }

    float getCameraSpeed() { return cameraSpeed; }
    void setCameraSpeed(float speed)
    {
        cameraSpeed = speed;
        if (cameraSpeed < 0.0f) {
            cameraSpeed = 0.0f;
        };
    }

private:
    FW::ref<FW::Shader> shader;
    FW::ref<Cube> testCube;
    FW::scope<FW::PlayerController> playerController;
    FW::ref<FW::CameraController> cameraController;
    float cameraSpeed = 15.0f;
    FW::Timer timer;

    FW::ref<Floor> grid;
    bool isRightButtonPressed = false;
};

FW::GLFWApplication*
FW::createApplication()
{
    return new PhysicsApp("Game", "1.0.0", { 1280, 720 });
}
