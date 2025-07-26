#pragma once

#include "Framework.h"
#include "Scene.h"

/**
 * The application class is primarily concerned about handling inputs.
 */
class JumpingPlatformerApp : public FW::GLFWApplication {
public:
    JumpingPlatformerApp() = default;
    JumpingPlatformerApp(const std::string& name,
                         const std::string& version,
                         glm::vec2 windowSize = glm::vec2(1280, 720))
      : GLFWApplication(name, version, windowSize) {}

    virtual ~JumpingPlatformerApp();

    /** Initialisation code goes here */
    virtual bool init();

    /** Contains the run loop */
    virtual void run();

    virtual void keyCallback(int key, int scancode, int action, int mods) {}
    virtual void cursorPosCallback(double xpos, double ypos) {}
    virtual void mouseButtonCallback(int button, int action, int mods) {}
    virtual void mouseScrollCallback(double xoffset, double yoffset) {}
    virtual void framebufferSizeCallback(int width, int height) {}

private:
    FW::ref<JumpingPlatformerScene> scene;
};

// The main function calls FW::createApplication(). We define this function here
// to return the application class itself.
FW::GLFWApplication* FW::createApplication() {
    return new JumpingPlatformerApp("Jumping Platformer", "1.0.0", { 1280, 720 });
}
