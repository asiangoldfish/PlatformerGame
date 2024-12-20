#include "App.h"

bool JumpingPlatformerApp::init() {
    if (!GLFWApplication::init()) {
        return false;
    }

    scene = FW::createRef<JumpingPlatformerScene>();
    scene->init();

    return true;
}

void JumpingPlatformerApp::run() {
    while (!glfwWindowShouldClose(getWindow())) {
        timer.updateDeltaTime();
        
        glfwPollEvents();

        RenderCommand::clear();

        physicsServer->update(timer.getDeltaTime());
        
        scene->update(timer.getDeltaTime());

        glfwSwapBuffers(getWindow());
        FW::Input::clearJustPressed();
    }
}

JumpingPlatformerApp::~JumpingPlatformerApp() {
    FW::TextureManager::clearTextures();
}