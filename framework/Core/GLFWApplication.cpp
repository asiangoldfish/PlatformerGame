// C++ libraries
#include <csignal>

// External libraries
#include <glad/glad.h>

// Framework
#include "GLFWApplication.h"
#include "Log.h"
#include "RenderCommands.h"
#include "Input.h"

static void FW_GLFWKey_Callback(GLFWwindow* window,
                                int key,
                                int scancode,
                                int action,
                                int mods);
static void FW_GLFWCursorPos_Callback(GLFWwindow* window,
                                      double xpos,
                                      double ypos);
static void FW_GLFWMouseButton_Callback(GLFWwindow* window,
                                        int button,
                                        int action,
                                        int mods);
static void FW_GLFWMouseScroll_Callback(GLFWwindow* window,
                                        double xoffset,
                                        double yoffset);
static void FW_GLFWFramebufferSize_Callback(GLFWwindow* window,
                                            int width,
                                            int height);
static void GLAPIENTRY OpenGL_DebugMessages(GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam);

static FW::GLFWApplication* gApp = nullptr;

namespace FW {
    // Set initial values to member variables
    GLFWApplication::GLFWApplication(const std::string& name,
                                     const std::string& version,
                                     glm::vec2 windowSize)
      : appName(name)
      , appVersion(version)
      , window(nullptr) {
        windowSettings.size = windowSize;
    }

    GLFWApplication::~GLFWApplication() {
        RenderCommand::destroy();

        // Destroy window and terminate GLFW.
        // NOTE: DO NOT run any GLFW functions after calling glfwTerminate()!!
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool GLFWApplication::init() {
        // ------------------
        // Initialize GLFW and create the application window.
        // ------------------

        if (!glfwInit()) {
            WARN("GLFW::INIT::Failed to initialize");
            return false;
        }

        // Create window
        window = glfwCreateWindow(windowSettings.size.x,
                                  windowSettings.size.y,
                                  appName.c_str(),
                                  nullptr,
                                  nullptr);

        if (!window) {
            WARN("GLFW::WINDOW::Unable to create window");
            return false;
        }

        // ------------------
        // Configure and setup window context
        // ------------------

        // GLFW hints
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 1);

        // Make window's context the current one. This also lets us load OpenGL
        // functions with GLAD using gladLoadGLLoader(...).
        // More about GLFW context:
        // https://computergraphics.stackexchange.com/a/4563
        glfwMakeContextCurrent(window);

        // Load OpenGL functions in runtime
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glEnable(GL_BLEND); // For transparency

        // Depth testing
        glEnable(GL_DEPTH_TEST);
        RenderCommand::setCurrentDepthFunc(GL_LESS);

        // Culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Uncomment this only for debugging
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGL_DebugMessages, nullptr);
        glDebugMessageControl(
          GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        // User input - Register the window.
        // These errors should never happen, but there is no harm in checking
        // them. Input::setWindow() returns 0 if everything is OK, and non-zero
        // integer if something went wrong.
        if (Input::setWindow(window)) {
            WARN("GLFWApplication::Init: Failed to set window. This should "
                 "never happen");
            return false;
        }

        // Input callbacks
        glfwSetKeyCallback(getWindow(), FW_GLFWKey_Callback);
        glfwSetCursorPosCallback(getWindow(), FW_GLFWCursorPos_Callback);
        glfwSetMouseButtonCallback(getWindow(), FW_GLFWMouseButton_Callback);
        glfwSetScrollCallback(getWindow(), FW_GLFWMouseScroll_Callback);
        glfwSetFramebufferSizeCallback(getWindow(),
                                       FW_GLFWFramebufferSize_Callback);

        // Static pointer to this application for setting up callback functions
        gApp = this;

        INFO("GLFW callback functions successfully bound");

        // Rendering
        RenderCommand::init();

        if (!configureDirectories()) {
            FATAL("Failed to configure directories");
        } else {
            INFO("Created game directories");
        }

        INFO("GLFWApplication \'{}\' successfully initiated", appName);

        return true;
    }

    void GLFWApplication::changeWindowMode(WindowMode mode) {
        // Safe guard for developer error in case the window does not exist.
        // We set a warning so they should create a window first. This improves
        // code readability.
        if (window == nullptr) {
            // Set default values to window size if they are uninitialised
            if (windowSettings.size.x == 0 || windowSettings.size.y == 0) {
                windowSettings.size = { 1280, 720 };
            }

            window = glfwCreateWindow(windowSettings.size.x,
                                      windowSettings.size.y,
                                      appName.c_str(),
                                      nullptr,
                                      nullptr);
        }

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* vidmode = glfwGetVideoMode(monitor);

        glm::vec2 posOnScreen = glm::vec2{ 0, 0 };

        switch (mode) {
            case WindowMode::WINDOW:
                /*  Order of priority:
                    1. Use the windowSize class member
                    2. Use default window size
                    3. Use hard coded window size
                */
                // We also set some threshold to prevent un-resizable tiny
                // windows
                if (windowSettings.size.x < 62.0f ||
                    windowSettings.size.y < 62.f) {
                    windowSettings.size = { 1280, 720 };
                }

                // TODO Center window on screen
                posOnScreen =
                  glm::vec2{ (vidmode->width / 2) - (windowSettings.size.x / 2),
                             (vidmode->height / 2) -
                               (windowSettings.size.y / 2) };
                monitor = nullptr;
                break;

            case WindowMode::BORDERLESS:
                // TODO Implement this
                WARN("WindowMode::BORDERLESS not implemented");
                windowSettings.size = { 1280, 720 };
                posOnScreen = glm::vec2{ 1920, 1280 };
                monitor = nullptr;
                break;

            case WindowMode::FULLSCREEN:
                // TODO Get monitor resolution
                windowSettings.size = { vidmode->width, vidmode->height };
                break;

            default:
                FATAL("GKFWApplication.cpp: WindowMode option not implemented");
                INFO("Error occurred. Please WindowMode option was not "
                     "implemented. Resetting to default values");
                window = glfwCreateWindow(windowSettings.size.x,
                                          windowSettings.size.y,
                                          appName.c_str(),
                                          nullptr,
                                          nullptr);
        }

        glfwSetWindowMonitor(window,
                             monitor,
                             posOnScreen.x,
                             posOnScreen.y,
                             windowSettings.size.x,
                             windowSettings.size.y,
                             vidmode->refreshRate);
    }

} // namespace Framework

/**
 * Called whenever the mouse position is changed.
 */
void FW_GLFWCursorPos_Callback(GLFWwindow* window, double xpos, double ypos) {
    gApp->cursorPosCallback(xpos, ypos);
}

/**
 * Called whenever the mouse's scroll wheel is used. For middle click,  see
 * <u>FW_GLFWMouseButton_Callback()</u>
 */
void FW_GLFWMouseScroll_Callback(GLFWwindow* window,
                                 double xoffset,
                                 double yoffset) {
    gApp->mouseScrollCallback(xoffset, yoffset);
}

/**
 * Called whenever a mouse button is pressed.
 */
void FW_GLFWMouseButton_Callback(GLFWwindow* window,
                                 int button,
                                 int action,
                                 int mods) {
    gApp->mouseButtonCallback(button, action, mods);
}

/**
 * Called whenever a key is pressed
 */
void FW_GLFWKey_Callback(GLFWwindow* window,
                         int key,
                         int scancode,
                         int action,
                         int mods) {
    gApp->keyCallback(key, scancode, action, mods);
}

/**
 * Called whenever the framebuffer size is changed.
 */
void FW_GLFWFramebufferSize_Callback(GLFWwindow* window,
                                     int width,
                                     int height) {
    // If the Editor uses the default frame buffer object, then this should be
    // uncommented.
    //
    // Update the default frame buffer object's size with the GLFW window size
    // gApp->framebufferSizeCallback(width, height);
}

// Output messages from OpenGL
static void GLAPIENTRY OpenGL_DebugMessages(GLenum source,
                                            GLenum type,
                                            GLuint id,
                                            GLenum severity,
                                            GLsizei length,
                                            const GLchar* message,
                                            const void* userParam) {
    std::cerr << "GL CALLBACK:"
              << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
              << "type = 0x" << type << ", severity = 0x" << severity
              << ", message =" << message << std::endl;
}
