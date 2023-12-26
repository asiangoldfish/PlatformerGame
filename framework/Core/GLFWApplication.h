#pragma once

#include "pch.h"

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace FW {
    /**
     * Application class
     *
     * @details The application class is the primary class for managing the
     * application runtime. There are three important components to the class.
     * <ul>
     *   <li><u>init()</u></i>
     *   <li><u>run()</u></li>
     *   <li><u>shutdown()</u></li>
     * </ul>
     *
     * @details <u>init()</u> handles initialization of the application and sets
     * all required OpenGL parameters to create a window. Edit this function in
     * <i>GLFWApplication.cpp</i> to change properties like minimum OpenGL
     * version, OpenGL error messages and rendering techniques like depth
     * testing.
     *
     * @details <u>run()</u> is a pure virtual method. It is implemented by the
     * user. A while-loop should be continuously looped until the application
     * quits.
     *
     * @details <u>shutdown()</u> is also a pure virtual method. It is called
     * after <u>run()</u> has returned and all memory deallocations and
     * deletions should happen here. Once thus function calls, it is expected
     * that <u>glfwTerminate()</u> is executed before the application's process
     * ends.
     *
     * @example Template to create a new application:
     * @code
     * #include "Framework.h"
     *
     * // Create a subclass
     * class MyApplication() : public GLFWApplication
     * {
     * public:
     *     void init() override
     *     {
     *         if (!GLFWApplication::init()) {
     *             return false;
     *         }
     *
     *         // Use the rest of the function to initialize data like models,
     * shaders or more.
     *
     *         RenderCommand::setClearColor(glm::vec3{ 0.5f });
     *
     *         INFO("Client application successfully initialized");
     *         return true;
     *     }
     *
     *     void run() override
     *     {
     *         // Runs until end of application
     *         while (!glfwWindowShouldClose(glfwWindow()) {
     *             // Clear the screen
     *             RenderCommand::clear();
     *             glfwPollEvents();
     *
     *             // Perform computations on per-frame basis
     *
     *             // Render geometries
     *
     *             // Swap buffers. This ensures the default framebuffer is
     * fully loaded before rendered glfwSwapBuffers(getWindow());
     *         }
     *     }
     *
     *     void shutdown() override
     *     {
     *         // Delete objects. Example:
     *         // DELETE(foo);
     *         // DELETE(fooArray, n);
     *     }
     * };
     *
     * int main() {
     *     auto app = new MyApplication("FooGame", "1.0.0", { 720, 360 });
     *     if (app->init()) {
     *         delete app;
     *         return 1;
     *     }
     *     app->run()
     *     app->shutdown()
     *     delete app;
     *     return 0;
     * }
     * @endcode
     */
    class GLFWApplication
    {
    public:
        /**
         * Default constructor for the Application.
         *
         * @details Sets window and application related data.
         *
         * @param name The application name. This will appear on the operating
         * system's status bar
         * @param version Application version
         */
        GLFWApplication(const std::string& name,
                        const std::string& version,
                        glm::vec2 windowSize = glm::vec2(1280, 720));

        /**
         * Release resources.
         *
         * @details Please note that this does not release resources allocated
         * in the GPU
         */
        virtual ~GLFWApplication();

        /**
         * Initialize the application.
         *
         * @details This has to be run once. The initialization may fail, so
         * always check the return value.
         *
         * @return True if successfully initialized. False if failed.
         */
        virtual bool init();

        /**
         * The main loop.
         *
         * @details When the loop ends, the application also ends. Clean up
         * process should happen after ending this loop
         */
        virtual void run() = 0;

        /**
         * Clean up resources before terminating GLFW and killing the
         * application process.
         */
        virtual void shutdown() = 0;

        /**
         * Return the window this application runs in.
         *
         * @details Currently the application supports only window.
         * @return The application window.
         */
        inline GLFWwindow* getWindow() const { return window; }

        /**
         * Return the window size. This is always the current size.
         *
         * @return The current window size.
         */
        const glm::vec2 getWindowSize() & { return windowSize; }

        /**
         * Set the new window size.
         *
         * @details Use this method whenever the <u>GLFWwindow</u> size changes.
         *
         * @param size The viewport's new size
         */
        void setWindowSize(const glm::vec2 size)
        {
            glViewport(0, 0, size.x, size.y);
            windowSize = size;
        }

    private:
        /** Application name. This is also the window class. */
        std::string appName;

        /** Application window */
        std::string appVersion;

        /** The application window where all rendering happens */
        GLFWwindow* window;

        /** The current window size. This is the <u>GLFWwindow</u>, not the viewport size */
        glm::vec2 windowSize;
    };
} // FW
