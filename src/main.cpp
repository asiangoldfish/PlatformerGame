/*
 * Notes:
 *  - Throughout the application, raw pointers is almost exclusively preferred over smart pointers. This is deliberate,
 *    because it helps to manage object lifetime and deallocating resources before glfwTerminate() is called.
 */

#include "SokobanApplication.h"

int main(int argc, char* argv[]) {
    /* Create a new instance of the application
     * By heap allocating the app, we can easier control its lifetime.
     */
    auto app = new SokobanApplication("Game", "1.0.0", { 1024, 1024 });
    if (!app->init()) {
        // If the application somehow fails to startup, we gracefully kill it.
        app->shutdown(); // Ensure everything is cleaned up before calling glfwTerminate()
        delete app; // This line of code will invoke GLFWApplication's destructor with glfwTerminate()
        return 1;
    }
    app->run(); // Game loop

    // Gracefully kill the application
    app->shutdown();
    delete app;

    return 0;
}