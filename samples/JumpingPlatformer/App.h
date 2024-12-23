#pragma once

#include "Framework.h"
#include "Scene.h"

class JumpingPlatformerApp : public GLFWApplication {
public:
    // The default constructor. Used by the compiler. We don't use this
    // constructor.
    JumpingPlatformerApp() = default;

    // We use this constructor to create our application class.
    JumpingPlatformerApp(const std::string& name,
                         const std::string& version,
                         glm::vec2 windowSize = glm::vec2(1280, 720))
      : GLFWApplication(name, version, windowSize) {}

    virtual ~JumpingPlatformerApp();

    // We can initialise memeber variables and configurations here.
    virtual bool init();

    // The game loop is executed here.
    virtual void run();

    // ------------
    // These callback methods are optional. They can be omitted.
    // ------------
    // Called whenever the user enters a keyboard input
    virtual void keyCallback(int key, int scancode, int action, int mods) {}

    // Called whenever the cursor is moved
    virtual void cursorPosCallback(double xpos, double ypos) {}

    // Called whenever a mouse button is pressed
    virtual void mouseButtonCallback(int button, int action, int mods) {}

    // Called whenever the mouse scroll wheel is used.
    virtual void mouseScrollCallback(double xoffset, double yoffset) {}

    // Called whenever the viewport is changed. This does not necessarily have
    // to be the window.
    virtual void framebufferSizeCallback(int width, int height) {}

private:
    // Almost everything happens inside scenes.
    ref<JumpingPlatformerScene> scene;
};

// This is called by the Framework's main function. This method creates a new
// instance of our application.
GLFWApplication* createApplication() {
    return new JumpingPlatformerApp("Jumping Platformer", "1.0.0", { 1280, 720 });
}
