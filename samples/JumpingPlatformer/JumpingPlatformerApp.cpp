#include "JumpingPlatformerApp.h"

JumpingPlatformerApp::~JumpingPlatformerApp() {
    FW::TextureManager::clearTextures();
}

bool JumpingPlatformerApp::init() {
    if (!GLFWApplication::init()) {
        return false;
    }

    return true;
}

void JumpingPlatformerApp::run() {
    timer.updateDeltaTime();

    while (!glfwWindowShouldClose(getWindow())) {
        timer.updateDeltaTime();
        
        glfwPollEvents();

        RenderCommand::clear(GL_DEPTH_BUFFER_BIT);
        
        // scene->update(timer.getDeltaTime());

        glfwSwapBuffers(getWindow());
        FW::Input::clearJustPressed();
    }
}

