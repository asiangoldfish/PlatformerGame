#include "GLFWApplication.h"

int
main(int argc, char* argv[])
{
    auto app = FW::createApplication();

    if (!app->init()) {
        delete app;
        return 1;
    }
    app->run(); // Game loop

    delete app;

    return 0;
}
