#pragma once

#include <string>
#include <memory>

#include "Framework.h"
#include "QtScene.h"

class QtApp : public GLFWApplication {
public:
    /**
     * Constructor. Remember to call init() before using the application.
     * @param name Window class and application name
     * @param version Application version
     */
    QtApp(const std::string& name,
               const std::string& version,
               glm::vec2 windowSize = glm::vec2(1024, 1024))
      : GLFWApplication(name, version, windowSize) {}

    virtual ~QtApp();

    /** Initialize the application. Must be called before using it */
    bool init() override;
    /** Game loop. Runs until the application quits. */
    void run() override;

    bool configureDirectories() override;

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

    Timer& getTimer() { return timer; }

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

    ref<Framebuffer> getViewportFramebuffer() {
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
    ref<Shader> shader;
    Timer timer;
    ref<Framebuffer> viewportFramebuffer;
    ref<JSONParser> editorConfig;

    ref<QtScene> scene;
};

GLFWApplication* createApplication() {
    return new QtApp("QtGame", "1.0.0", { 1280, 720 });
}
