// C++ libraries
#include <csignal>

// External libraries
#include <glad/glad.h>

// Framework
#include "GLFWApplication.h"
#include "Log.h"
#include "RenderCommands.h"
#include "Input.h"

static void
FW_GLFWKey_Callback(GLFWwindow* window,
                    int key,
                    int scancode,
                    int action,
                    int mods);
static void
FW_GLFWCursorPos_Callback(GLFWwindow* window, double xpos, double ypos);
static void
FW_GLFWCursorMouseButton_Callback(GLFWwindow* window,
                                  int button,
                                  int action,
                                  int mods);
static void
FW_GLFWMouseScroll_Callback(GLFWwindow* window, double xoffset, double yoffset);
static void
FW_GLFWFramebufferSize_Callback(GLFWwindow* window, int width, int height);

static FW::GLFWApplication* gApp = nullptr;

namespace FW {
    // Output messages from OpenGL
    static void GLAPIENTRY messageCallback(GLenum source,
                                           GLenum type,
                                           GLuint id,
                                           GLenum severity,
                                           GLsizei length,
                                           const GLchar* message,
                                           const void* userParam)
    {
        std::cerr << "GL CALLBACK:"
                  << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
                  << "type = 0x" << type << ", severity = 0x" << severity
                  << ", message =" << message << std::endl;
    }

    // Set initial values to member variables
    GLFWApplication::GLFWApplication(const std::string& name,
                                     const std::string& version,
                                     glm::vec2 windowSize)
      : appName(name)
      , appVersion(version)
      , window(nullptr)
      , windowSize(windowSize)
    {
    }

    GLFWApplication::~GLFWApplication()
    {
        // Destroy window and terminate GLFW.
        // NOTE: DO NOT run any GLFW functions after calling glfwTerminate()!!
        glfwDestroyWindow(window);
        glfwTerminate();
    }

    bool GLFWApplication::init()
    {
        // ------------------
        // Initialize GLFW and create the application window.
        // ------------------

        if (!glfwInit()) {
            std::cerr << "GLFW::INIT::Failed to initialize\n";
            return false;
        }

        // Create window
        window = glfwCreateWindow(
          windowSize.x, windowSize.y, appName.c_str(), nullptr, nullptr);

        if (!window) {
            std::cerr << "GLFW::WINDOW::Unable to create window\n";
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
        glDebugMessageCallback(messageCallback, nullptr);
        glDebugMessageControl(
          GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        // User input - Register the window.
        // These errors should never happen, but there is no harm in checking
        // them. Input::setWindow() returns 0 if everything is OK, and non-zero
        // integer if something went wrong.
        if (Input::setWindow(window)) {
            FATAL("GLFWApplication::Init: Failed to set window. This should "
                  "never happen");
            return false;
        }

        // Input callbacks
        glfwSetKeyCallback(getWindow(), FW_GLFWKey_Callback);
        glfwSetCursorPosCallback(getWindow(), FW_GLFWCursorPos_Callback);
        glfwSetMouseButtonCallback(getWindow(),
                                   FW_GLFWCursorMouseButton_Callback);
        glfwSetScrollCallback(getWindow(), FW_GLFWMouseScroll_Callback);
        glfwSetFramebufferSizeCallback(getWindow(),
                                       FW_GLFWFramebufferSize_Callback);

        // Static pointer to this application for setting up callback functions
        gApp = this;

        INFO("GLFW callback functions successfully bound");

        INFO("GLFWApplication \'{}\' successfully initiated", appName);

        return true;
    }

} // namespace Framework

// Source: https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
void
FW_GLFWCursorPos_Callback(GLFWwindow* window, double xpos, double ypos)
{
    gApp->cursorPosCallback(xpos, ypos);
}

void
FW_GLFWMouseScroll_Callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gApp->mouseScrollCallback(xoffset, yoffset);
}

// Source: https://www.glfw.org/docs/3.0/group__input.html
void
FW_GLFWCursorMouseButton_Callback(GLFWwindow* window,
                                  int button,
                                  int action,
                                  int mods)
{
    gApp->mouseButtonCallback(button, action, mods);
}

/* Keyboard input function. Used as callback function and is used in
 * OpenGL's input polling system. */
void
FW_GLFWKey_Callback(GLFWwindow* window,
                    int key,
                    int scancode,
                    int action,
                    int mods)
{
    gApp->keyCallback(key, scancode, action, mods);
}

void
FW_GLFWFramebufferSize_Callback(GLFWwindow* window, int width, int height)
{
    gApp->framebufferSizeCallback(width, height);
}
