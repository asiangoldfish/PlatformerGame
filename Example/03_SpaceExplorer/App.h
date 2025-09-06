#pragma once

#include "Framework.h"
#include "Scene.h"

#include "Debugging.h"

class GameApplication : public FW::GLFWApplication {
public:
    GameApplication() = default;
    GameApplication(const std::string& name,
                    const std::string& version,
                    glm::vec2 windowSize = glm::vec2(1280, 720))
      : GLFWApplication(name, version, windowSize) {}
    virtual ~GameApplication();
    virtual bool init();
    virtual void run();

    virtual void keyCallback(int key, int scancode, int action, int mods);
    virtual void cursorPosCallback(double xpos, double ypos);
    virtual void mouseButtonCallback(int button, int action, int mods);

private:
    FW::ref<GameScene> scene;

    FW::ref<Debugging> debugging;
};

FW::GLFWApplication* FW::createApplication() {
    return new GameApplication("Space Explorer", "1.0.0", { 1280, 720 });
}
