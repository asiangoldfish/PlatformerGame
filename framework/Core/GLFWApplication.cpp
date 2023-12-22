#include "GLFWApplication.h"

#include <glad/glad.h>

#include <iostream>
#include <csignal>

#include "assertions.h"

#include "Log.h"

namespace Framework {
    // Output messages from OpenGL
    static void GLAPIENTRY messageCallback(GLenum source, GLenum type,
        GLuint id, GLenum severity,
        GLsizei length,
        const GLchar* message,
        const void* userParam) {
        std::cerr << "GL CALLBACK:"
            << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "")
            <<
            "type = 0x" << type <<
            ", severity = 0x" << severity <<
            ", message =" << message << std::endl;
    }

    // Set initial values to member variables
    GLFWApplication::GLFWApplication(
        const std::string& name,
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
//        std::cout << "Destructor: GLFW App" << std::endl;
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
                windowSize.x,
                windowSize.y,
                appName.c_str(),
                nullptr,
                nullptr);

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
        //More about GLFW context: https://computergraphics.stackexchange.com/a/4563
        glfwMakeContextCurrent(window);

        // Load OpenGL functions in runtime
        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

        glEnable(GL_BLEND); // For transparency
        glEnable(GL_DEPTH_TEST);

        // Culling
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glFrontFace(GL_CCW);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        
        // Uncomment this only for debugging
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(messageCallback, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        INFO("Application {} initiated", appName);

        return true;
    }

} // namespace Framework
