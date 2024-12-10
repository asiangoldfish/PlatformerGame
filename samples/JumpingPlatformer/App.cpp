#include "App.h"

JumpingPlatformerApp::~JumpingPlatformerApp() {
    FW::TextureManager::clearTextures();
}

bool JumpingPlatformerApp::init() {
    if (!GLFWApplication::init()) {
        return false;
    }

    scene = FW::createRef<JumpingPlatformerScene>();
    scene->init();

    return true;
}

void JumpingPlatformerApp::run() {
    timer.updateDeltaTime();

    while (!glfwWindowShouldClose(getWindow())) {
        timer.updateDeltaTime();
        
        glfwPollEvents();

        RenderCommand::clear();
        
        scene->update(timer.getDeltaTime());

        glfwSwapBuffers(getWindow());
        FW::Input::clearJustPressed();
    }
}

