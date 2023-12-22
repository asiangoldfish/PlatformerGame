#ifndef __GLFW_APPLICATION_H__
#define __GLFW_APPLICATION_H__

#include <string>

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Framework {
    /**
     * Application class
     *
     * The GLFWApplication creates a window that the application lives in.
     */
    class GLFWApplication {
    public:
        /**
         * Constructor for the Application
         *
         * @param name The application name. This will appear on the operating
         * system's status bar.
         * @param version Application version.
         */
        GLFWApplication(
            const std::string& name,
            const std::string& version,
            glm::vec2 windowSize = glm::vec2(1280, 720));

        /**
         * Release resources.
         *
         * Please note that this does not release resources allocated in the GPU
         */
        virtual ~GLFWApplication();

        /**
         * Initialize the application.
         *
         * This has to be run once. The
         * initialization may fail, so always check the return value.
         * @return True if successfully intialized. False if failed.
         */
        virtual bool init();

        /**
         * The main loop.
         *
         * When the loop ends, the application also ends. Clean up process
         * should happen after ending this loop.
         */
        virtual void run() = 0;

        virtual void shutdown() = 0;

        /**
         * The window this application runs in.
         *
         * Currently the application supports only window.
         * @return The application window.
         */
        inline GLFWwindow* getWindow() const { return window; }

        const glm::vec2 getWindowSize()& { return windowSize; }
        void setWindowSize(const glm::vec2 size) { windowSize = size; }

    private:
        /** Application name. This is also the window class. */
        std::string appName;

        /** Application window */
        std::string appVersion;

        /** The application window where all rendering happens */
        GLFWwindow* window;

        glm::vec2 windowSize;
    };
}

#endif  // __GLFW_APPLICATION_H__
