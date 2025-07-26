#include "App.h"

bool JumpingPlatformerApp::init() {
    // This must be called. GLFWApplication::init() handles low level
    // initialisation.
    if (!GLFWApplication::init()) {
        return false;
    }

    // A scene is the application's content presented on the GUI.
    scene = FW::createRef<JumpingPlatformerScene>();
    scene->init();

    return true;
}

void JumpingPlatformerApp::run() {
    while (!glfwWindowShouldClose(getWindow())) {
        // Most mechanics in the application requires some kind of time delta.
        // This is updated once per frame.
        timer.updateDeltaTime();
        
        // Poll inputs. Without this, no inputs are handled.
        glfwPollEvents();

        // Clear the scene. Use RenderCommand::setClearColor() to change the
        // background color.
        RenderCommand::clear();

        // Increase the time step of the physics server. Its "run loop" is
        // called once per n seconds. Change this by calling
        // physicsServer->setDelta()
        // Currently, the physics engine cannot be disabled. Removing the
        // physics simulation is simply done by commenting the below line.
        physicsServer->update();
        
        // Update the scene's run loop. This happens once per frame.
        scene->update(timer.getDeltaTime());

        // Render stuff to the screen.
        glfwSwapBuffers(getWindow());

        FW::Input::clearJustPressed();
    }
}

JumpingPlatformerApp::~JumpingPlatformerApp() {
    FW::TextureManager::clearTextures();
}