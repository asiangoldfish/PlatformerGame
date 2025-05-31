
#include "QtApp.h"

bool QtApp::init() {
    // ------
    // Configure application
    // ------
    if (!GLFWApplication::init()) {
        return false;
    }

    INFO("Client application successfully initialized");

    return true;
}

void QtApp::run() {
    Timer emitterTimer;
    emitterTimer.resetTimer();

    while (!glfwWindowShouldClose(getWindow())) {
        glfwPollEvents();

        viewportFramebuffer->bind(); // Render graphics on a separate viewport
        RenderCommand::clear();

        keyboardInput();
        timer.updateDeltaTime();

        // Below line must happen after drawing the world grid because of depth
        RenderCommand::clear(GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(getWindow());
        Input::clearJustPressed();
    }
}

bool QtApp::configureDirectories() {
    std::filesystem::create_directory(RESOURCES_DIR);
    std::filesystem::create_directory(CONFIG_DIR);

    return true;
}
/** Keyboard input function. Called every frame */
void QtApp::keyboardInput() {}

QtApp::~QtApp() {}

void QtApp::keyCallback(int key, int scancode, int action, int mods) {
    Input::updateJustPressed(key, action);

    // Update the key press state for left and right SHIFT, ALT, SUPER and
    // CONTROL
    Input::updateModKeyState(key, action);
}

void QtApp::cursorPosCallback(double xpos, double ypos) {}

void QtApp::mouseButtonCallback(int button, int action, int mods) {}

void QtApp::mouseScrollCallback(double xoffset, double yoffset) {}

void QtApp::framebufferSizeCallback(int width, int height) {}
