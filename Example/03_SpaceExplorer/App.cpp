#include "App.h"

bool GameApplication::init() {
    if (!GLFWApplication::init()) {
        return false;
    }

    setDepthTesting(false);

    scene = FW::createRef<GameScene>();
    scene->init();

    RenderCommand::setClearColor(glm::vec3{0.5f, 0.25f, 0.4f});

    return true;
}

void GameApplication::run() {
    while (!glfwWindowShouldClose(getWindow())) {
        timer.updateDeltaTime();
        glfwPollEvents();
        RenderCommand::clear();
        scene->update(timer.getDeltaTime());
        glfwSwapBuffers(getWindow());
        FW::Input::clearJustPressed();
    }
}

GameApplication::~GameApplication() {
    FW::TextureManager::clearTextures();
}

void GameApplication::keyCallback(int key, int scancode, int action, int mods) {
    // Quit application
    if (FW::Input::isKeyPressed(GLFW_KEY_ESCAPE)) {
        glfwSetWindowShouldClose(getWindow(), 1);
    }

    scene->keyCallback(key, scancode, action, mods);
}

void GameApplication::cursorPosCallback(double xpos, double ypos) {
    scene->cursorPosCallback(xpos, ypos);
}

void GameApplication::mouseButtonCallback(int button, int action, int mods) {
    scene->mouseButtonCallback(button, action, mods);
}
