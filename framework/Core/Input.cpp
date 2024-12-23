#include <GLFW/glfw3.h>

#include "Input.h"

Input::KeyRegister Input::registerKeyJustPressed;
Input::MouseRegister Input::registerMouseJustPressed;
GLFWwindow* Input::window = nullptr;
uint32_t Input::currentKeyState = 0;

bool Input::isKeyPressed(int keycode) {
    auto state = glfwGetKey(window, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::isKeyJustPressed(int keycode) {
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

bool Input::isMouseButtonPressed(int mouseButton) {
    auto state = glfwGetMouseButton(window, mouseButton);
    return state == GLFW_PRESS;
}

bool Input::isModKeyCombinationPressed(FW_KEY_BIT keys) {
    return currentKeyState == keys;
}

float Input::getMouseX() {
    return getMousePosition().x;
}

float Input::getMouseY() {
    return getMousePosition().y;
}

glm::vec2 Input::getMousePosition() {
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    return { xpos, ypos };
}

void Input::updateJustPressed(int key, int action) {
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

void Input::clearJustPressed() {
    // Mouse buttons
    registerMouseJustPressed.middleButton = false;
    registerMouseJustPressed.rightButton = false;
    registerMouseJustPressed.leftButton = false;

    // Keys
    registerKeyJustPressed.leftShift = false;
    registerKeyJustPressed.leftControl = false;
    registerKeyJustPressed.leftAlt = false;
}

bool Input::setWindow(GLFWwindow* win) {
    if (!win) {
        return false;
    }

    window = win;
    return true;
}

void Input::updateModKeyState(int key, int action) {
    /*
     * Only register the following left or right keys:
     * - Control
     * - Alt
     * - Shift
     * - Super
     */

    uint32_t bitkey = 0;

    switch (key) {
        case FW_KEY_LEFT_SHIFT:
            bitkey = FW_KEY_LEFT_SHIFT_BIT;
            break;
        case FW_KEY_RIGHT_SHIFT:
            bitkey = FW_KEY_RIGHT_SHIFT_BIT;
            break;
        case FW_KEY_LEFT_CONTROL:
            bitkey = FW_KEY_LEFT_CONTROL_BIT;
            break;
        case FW_KEY_RIGHT_CONTROL:
            bitkey = FW_KEY_RIGHT_CONTROL_BIT;
            break;
        case FW_KEY_LEFT_ALT:
            bitkey = FW_KEY_LEFT_ALT_BIT;
            break;
        case FW_KEY_RIGHT_ALT:
            bitkey = FW_KEY_RIGHT_ALT_BIT;
            break;
        case FW_KEY_LEFT_SUPER:
            bitkey = FW_KEY_LEFT_SUPER_BIT;
            break;
        case FW_KEY_RIGHT_SUPER:
            bitkey = FW_KEY_RIGHT_SUPER_BIT;
            break;
        default:
            bitkey = 0;
            break;
    }

    // If bitkey is 0, then no valid key was passed.
    if (!bitkey) {
        return;
    }

    if (action == GLFW_REPEAT || action == GLFW_PRESS) {
        currentKeyState |= FW_KEY_BIT(bitkey);
    } else {
        currentKeyState &= ~FW_KEY_BIT(bitkey);
    }
}
