#include <GLFW/glfw3.h>

#include "Input.h"

namespace FW {
    Input::KeyRegister Input::registerKeyJustPressed;
    Input::MouseRegister Input::registerMouseJustPressed;
    GLFWwindow* Input::window = nullptr;

    bool Input::isKeyPressed(int keycode)
    {
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool Input::isKeyJustPressed(int keycode)
    {
        switch (keycode) {
            case FW_KEY_LEFT_ALT:
                return registerKeyJustPressed.leftAlt;
            case FW_KEY_LEFT_CONTROL:
                return registerKeyJustPressed.leftControl;
            case FW_KEY_LEFT_SHIFT:
                return registerKeyJustPressed.leftShift;
            default:
                return false;
        }
    }

    bool Input::isMouseButtonPressed(int mouseButton)
    {
        auto state = glfwGetMouseButton(window, mouseButton);
        return state == GLFW_PRESS;
    }

    float Input::getMouseX()
    {
        return getMousePosition().x;
    }

    float Input::getMouseY()
    {
        return getMousePosition().y;
    }

    glm::vec2 Input::getMousePosition()
    {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { xpos, ypos };
    }

    void Input::updateJustPressed(int key, int action)
    {
        // ----------
        // Mouse
        // ----------
        // Left alt
        if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS &&
            !registerKeyJustPressed.leftAlt) {
            registerKeyJustPressed.leftAlt = true;
        } else if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
            registerKeyJustPressed.leftAlt = false;
        }
        // Left ctrl
        if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS &&
            !registerKeyJustPressed.leftControl) {
            registerKeyJustPressed.leftControl = true;
        } else if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
            registerKeyJustPressed.leftControl = false;
        }
        // Left shift
        if (registerKeyJustPressed.leftShift) {
            registerKeyJustPressed.leftShift = false;
        }
        if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS &&
            !registerKeyJustPressed.leftShift) {
            registerKeyJustPressed.leftShift = true;
        }

        // -----------
        // Keys
        // -----------
    }

    void Input::clearJustPressed()
    {
        // Mouse buttons
        registerMouseJustPressed.middleButton = false;
        registerMouseJustPressed.rightButton = false;
        registerMouseJustPressed.leftButton = false;

        // Keys
        registerKeyJustPressed.leftShift = false;
        registerKeyJustPressed.leftControl = false;
        registerKeyJustPressed.leftAlt = false;
    }

    int Input::setWindow(GLFWwindow* win)
    {
        // Window is already set.
        if (window) {
            return 1;
        }

        // w is a nullptr
        if (!win) {
            return 2;
        }

        window = win;
        return 0;
    }
}
