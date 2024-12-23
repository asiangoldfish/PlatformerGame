#include "App.h"

// Uncomment the below to try testing
// #include "doctest/doctest.h"

// // Example test with doctest
// float add(float x, float b) {
//     return x + b;
// }

// TEST_CASE("testing add x and b") {
//     CHECK(add(1,2) == 3);
// }

bool JumpingPlatformerApp::init() {
    if (!GLFWApplication::init()) {
        return false;
    }

    scene = createRef<JumpingPlatformerScene>();
    scene->init();

    return true;
}

void JumpingPlatformerApp::run() {
    while (!glfwWindowShouldClose(getWindow())) {
        timer.updateDeltaTime();
        
        glfwPollEvents();

        RenderCommand::clear();

        scene->update(timer.getDeltaTime());

        glfwSwapBuffers(getWindow());
        Input::clearJustPressed();
    }
}

JumpingPlatformerApp::~JumpingPlatformerApp() {
    TextureManager::clearTextures();
}
