#pragma once

#include "pch.h"

#define GLFW_INCLUDE_NONE

// External libraries
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace FW {
    enum class WindowMode {
        WINDOW = 0,
        BORDERLESS,
        FULLSCREEN
    };

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
         * Configure directories.
         * 
         * Create or configure directories where game or engine files are
         * located.
         */
        virtual bool configureDirectories() {};

        /**
         * The main loop.
         *
         * @details When the loop ends, the application also ends. Clean up
         * process should happen after ending this loop
         */
        virtual void run() = 0;

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

        /**
         * Callback function for when a keyboard key is interacted with.
         *
         * @details This function activates whenever an action like pressing or
         * releasing a key is performed by the user For more information, visit
         * the <i>'Key input'</i> section at <a
         * href="https://www.glfw.org/docs/3.3/input_guide.html#input_key">glfw.org</a>
         *
         * @param key The key that is pressed. Refer to the key codes at <a
         * href="https://github.com/asiangoldfish/PlatformerGame/blob/main/framework/Core/KeyCodes.h">
         * KeyCodes</a>.
         * @param scancode Platform specific code determining what key was
         * interacted with.
         * @param action Either <i>GLFW_PRESS</i>, <i>GLFW_REPEAT</i> or
         * <i>GLFW_RELEASE</i>. Determines how the key is interacted with.
         * @param mods Bitfield representing mod keys that were pressed.
         * Includes SHIFT, ALT, CONTROL and SUPER.
         */
        virtual void keyCallback(int key, int scancode, int action, int mods) {}

        /**
         * Callback function for when the mouse cursor moves across the
         * viewport.
         *
         * @details The function is called whenever the cursor changes position
         * within the viewport. Currently, only one viewport is supported. This
         * covers the whole client window.
         * @param xpos The cursor's X position
         * @param ypos The cursor's Y position
         */
        virtual void cursorPosCallback(double xpos, double ypos) {}

        /**
         * Callback function for when mouse buttons are pressed.
         *
         * @details The function is called whenever a mouse button is pressed,
         * released or held down.
         *
         * @param button The mouse button that is interacted with.
         * Refer to <a
         * href="https://github.com/asiangoldfish/PlatformerGame/blob/main/framework/Core/MouseButtonCodes.h">MouseButtonCodes.h</a>
         * for button macros.
         * @param action Either <i>GLFW_PRESS</i>, <i>GLFW_REPEAT</i> or
         * <i>GLFW_RELEASE</i>. Determines how the key is interacted with.
         * @param mods Bitfield representing mod keys that were pressed.
         * Includes SHIFT, ALT, CONTROL and SUPER.
         */
        virtual void mouseButtonCallback(int button, int action, int mods) {}

        /**
         * Callback function for when the mouse's scroll wheel is interacted
         with.

         * @param xoffset The mouse scrolling in the X-axis.
         * @param yoffset The mouse's scrolling in the Y-axis. This is the most
         commonly used value.
         */
        virtual void mouseScrollCallback(double xoffset, double yoffset) {}

        /**
         * Callback function for when the framebuffer's size changes.
         *
         * @details The function is called whenever the size changes. By
         * default, GLFW's default framebuffer covers the window, using its
         * initial size when initiating GLFW. If another framebuffer is used,
         * like a texture, this function is called whenever the texture size
         * changes.
         * @param width The framebuffer's new width.
         * @param height The framebuffer's new height
         */
        virtual void framebufferSizeCallback(int width, int height) {}

        void changeWindowMode(WindowMode mode);

    private:
        /** Application name. This is also the window class. */
        std::string appName;

        /** Application window */
        std::string appVersion;

        /** The application window where all rendering happens */
        GLFWwindow* window = nullptr;

        /** The current window size. This is the <u>GLFWwindow</u>, not the
         * viewport size */
        glm::vec2 windowSize;

        /** Default window mode upon launch */
        WindowMode windowMode = WindowMode::WINDOW;
    };

    GLFWApplication* createApplication();
} // FW
