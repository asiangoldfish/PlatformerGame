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
        // Create window
        createWindow(false);

        setWindowBlendMode();

        // Uncomment this only for debugging
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(OpenGL_DebugMessages, nullptr);
        glDebugMessageControl(
          GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

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

        /* Physics */
        // TODO: Remove. Clients may not want to implement physics.
        physicsServer = createScope<Physics::PhysicsServer>();
        physicsServer->stepSize = 1;

        /* Time */
        timer.updateDeltaTime();

        INFO("GLFWApplication \'{}\' successfully initiated", appName);

        return true;
    }

    void GLFWApplication::changeWindowMode(WindowMode mode) {
        const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());

        bool isFullscreen = false;

        switch (mode) {
            case WindowMode::WINDOW:
                glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
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
                break;

            case WindowMode::BORDERLESS:
                // Requirements:
                // - Windows size = monitor size
                // - Not movable
                windowSettings.size = { vidmode->width, vidmode->height };
                glfwWindowHint(GLFW_DECORATED, GL_FALSE);
                break;

            case WindowMode::FULLSCREEN:
                glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
                windowSettings.size = { vidmode->width, vidmode->height };
                isFullscreen = true;
                break;

            default:
                // This should never happen
                FATAL("Window mode is not implemented. Did you forget to "
                      "implement it?");

                // TODO Send error message to the editor and create default
                // window
                break;
        }

        // Making window not resizable requires recreating the window
        createWindow(isFullscreen);
    }

    void GLFWApplication::createWindow(bool isFullscreen) {
        if (window) {
            glfwDestroyWindow(window);
        }

        // For fullscreen mode
        // FIXME this is not correctly implemented fullscreen mode
        GLFWmonitor* monitor =
          (isFullscreen) ? glfwGetPrimaryMonitor() : nullptr;

        window = glfwCreateWindow(windowSettings.size.x,
                                  windowSettings.size.y,
                                  appName.c_str(),
                                  monitor,
                                  nullptr);

        if (!window) {
            FATAL("GLFW::WINDOW::Unable to create window");
        }

        // Center window on screen
        const GLFWvidmode* vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        glm::vec2 newPos =
          glm::vec2{ (vidmode->width / 2) - (windowSettings.size.x / 2),
                     (vidmode->height / 2) - (windowSettings.size.y / 2) };
        glfwSetWindowPos(window, newPos.x, newPos.y);
        glfwMakeContextCurrent(window);
        setWindowBlendMode();

        // User input - Register the window.
        // If this returns false, then there is a developer error somewhere.
        if (!Input::setWindow(window)) {
            FATAL("GLFWApplication::Init: Failed to set window. This should "
                  "never happen");
        }

        // Register input callbacks
        glfwSetKeyCallback(getWindow(), FW_GLFWKey_Callback);
        glfwSetCursorPosCallback(getWindow(), FW_GLFWCursorPos_Callback);
        glfwSetMouseButtonCallback(getWindow(), FW_GLFWMouseButton_Callback);
        glfwSetScrollCallback(getWindow(), FW_GLFWMouseScroll_Callback);
        glfwSetFramebufferSizeCallback(getWindow(),
                                       FW_GLFWFramebufferSize_Callback);
    }

    void GLFWApplication::setWindowBlendMode() {
        if (!window) {
            FATAL("Window is NULL");
        }

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
    gApp->framebufferSizeCallback(width, height);
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
