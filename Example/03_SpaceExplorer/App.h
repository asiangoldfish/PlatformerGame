#pragma once

#include "Framework.h"
#include "Scene.h"

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

private:
    FW::ref<GameScene> scene;
};

FW::GLFWApplication* FW::createApplication() {
    return new GameApplication("Space Explorer", "1.0.0", { 1280, 720 });
}
