#include "GLFWApplication.h"

#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, char* argv[]) {
    // Configure doctest
    int res = 0;

#ifndef DOCTEST_CONFIG_DISABLE
    doctest::Context context;
    res = context.run();

    context.applyCommandLine(argc, argv);

    if (context.shouldExit()) // important - query flags (and --exit) rely on
                              // the user doing this propagate the result of the
                              // tests
        return res;
#endif

    auto app = FW::createApplication();

    if (!app->init()) {
        delete app;
        return res + 1;
    }
    app->run(); // Game loop

    delete app;

    return res;
}
