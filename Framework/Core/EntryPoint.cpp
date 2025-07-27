#include "GLFWApplication.h"

int
main(int argc, char* argv[])
{
    bool shouldRestart = false;
    do {

        auto app = FW::createApplication();
        
        if (!app->init()) {
            delete app;
            return 1;
        }
        app->run(); // Game loop
        shouldRestart = app->shouldRestartItself;
        
        delete app;
    } while (shouldRestart);

    return 0;
}
