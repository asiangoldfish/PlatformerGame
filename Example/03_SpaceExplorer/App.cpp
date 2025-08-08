#include "App.h"

bool GameApplication::init() {
    if (!GLFWApplication::init()) {
        return false;
    }

    scene = FW::createRef<GameScene>();
    scene->init();

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