#pragma once

#include <string>
#include <memory>

#include "Framework.h"
#include "Widgets/AppWidget.h"

#include "PhysicsScene.h"

class Cube;
class Floor;
class WorldGrid;

namespace FW {
    class Shader;
    class PerspectiveCamera;
    class TextureManager;
}

class PhysicsApp : public FW::GLFWApplication {
public:
    /**
     * Constructor. Remember to call init() before using the application.
     * @param name Window class and application name
     * @param version Application version
     */
    PhysicsApp(const std::string& name,
               const std::string& version,
               glm::vec2 windowSize = glm::vec2(1024, 1024))
      : GLFWApplication(name, version, windowSize) {}

    virtual ~PhysicsApp();

    /** Initialize the application. Must be called before using it */
    bool init() override;
    /** Game loop. Runs until the application quits. */
    void run() override;

    bool configureDirectories() override;

    // ---------
    // Camera and rendering
    // ---------
    // Camera
    [[nodiscard]] FW::ref<FW::CameraController>& getCameraController() {
        return cameraController;
    }

    [[nodiscard]] FW::ref<FW::Shader> getShader() { return shader; }

    // --------
    // Inputs
    // --------
    /** Called every frame */
    void keyboardInput();

    [[nodiscard]] bool getIsRightButtonPressed() const {
        return isRightButtonMousePressed;
    }
    void setIsRightButtonPressed(const bool isPressed) {
        isRightButtonMousePressed = isPressed;
    }

    FW::Timer& getTimer() { return timer; }

    float getCameraSpeed() { return cameraSpeed; }
    void setCameraSpeed(float speed) {
        cameraSpeed = speed;
        if (cameraSpeed < 0.0f) {
            cameraSpeed = 0.0f;
        };
    }

    void centralizeCursorInWindow() {
        glfwSetCursorPos(
          getWindow(), windowSettings.size.x / 2, windowSettings.size.y / 2);
    }

    virtual void keyCallback(int key,
                             int scancode,
                             int action,
                             int mods) override;
    virtual void cursorPosCallback(double xpos, double ypos) override;
    virtual void mouseButtonCallback(int button, int action, int mods) override;
    virtual void mouseScrollCallback(double xoffset, double yoffset) override;
    virtual void framebufferSizeCallback(int width, int height) override;

    FW::ref<FW::Framebuffer> getViewportFramebuffer() {
        return viewportFramebuffer;
    }

public:
    /** Used by AppWidget to force right mouse down */
    bool overrideRightMouseDown = false;
    bool isRightButtonMousePressed = false;
    bool isLeftButtonMousePressed = false;
    bool isLeftAltPressed = false;
    bool isLeftCtrlPressed = false;
    bool isLeftShiftPressed = false;
    glm::vec2 savedCursorPosition{ 0.0f };
    glm::vec2 savedCameraPosition{ 0.0f };

    float cameraDistance = 0.0f;
    float cameraRotationSpeed = 1.0f;
    float cameraCurrentYaw = 0.0f;
    float cameraCurrentPitch = 0.0f;

private:
    FW::ref<FW::Shader> shader;
    FW::scope<FW::PlayerController> playerController;
    FW::ref<FW::CameraController> cameraController;
    float cameraSpeed = 15.0f;
    FW::Timer timer;

    FW::ref<FW::Framebuffer> viewportFramebuffer;

    FW::ref<FW::Shader> worldGridShader;
    FW::ref<WorldGrid> worldGrid;

    FW::ref<FW::JSONParser> editorConfig;

    AppWidget appWidget;

    FW::ref<PhysicsScene> scene;

    /** The Application class is the one who owns the filesystem. */
    FW::ref<Filesystem> filesystem;
};

FW::GLFWApplication* FW::createApplication() {
    return new PhysicsApp("Game", "1.0.0", { 1280, 720 });
}
