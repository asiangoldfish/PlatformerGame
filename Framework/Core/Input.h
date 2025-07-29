#pragma once

#include "pch.h"

// External libraries
#include <glm/glm.hpp>

// Framework
#include "MouseButtonCodes.h"
#include "KeyCodes.h"

namespace FW {
    class Input {
    public:
        /**
         * Determine what mouse buttons are held or released.
         *
         * @details The struct is for general use. Implementations determine how
         * the struct is used. It can for instance be used to determine whether
         * a mouse button is held down and not permit any other buttons while
         * it's held.
         */
        struct MouseRegister {
            bool rightButton = false;
            bool leftButton = false;
            bool middleButton = false;
        };

        /**
         * Determine what keyboard key are held or released.
         *
         * @details The struct is for general use. Implementations determine how
         * the struct is used. It can for instance be used to determine whether
         * a key is held down and not permit any other keys while it's held.
         */
        struct KeyRegister {
            bool leftAlt = false;
            bool leftControl = false;
            bool leftShift = false;
        };

    public:
        /**
         * Update the registration of keys that are just pressed.
         *
         * @details For performance, this function should only be called once
         * per frame. Consider placing it at the top of the game loop.
         *
         * @example
         * @code
         * #include "Framework.h"
         *
         * void keyCallback(GLFWwindow* window, int key, int scancode, int
         * action, int mods)
         * {
         *     FW::Input::updateJustPressed(key, action);
         * }
         *
         * int main() {
         *     // ...
         *     glfwSetKeyCallback(window, keyCallback);
         *
         *     // To find if 'Q' was just pressed
         *     if (FW::Input::isKeyJustPressed(FW_KEY_Q) {
         *         INFO("Key 'Q' just pressed!");
         * }
         * @endcode
         */
        static void updateJustPressed(int key, int action);
        static void updateModKeyState(int key, int action);

        /**
         * Clear all just-pressed variables. This should be called at the end of
         * the game loop.
         */
        static void clearJustPressed();

        /**
         * Check whether the key is pressed. This also checks if it's also
         * continuously held down.
         *
         * @details It's recommended to use the key codes from
         * <i><u>KeyCodes.h</u></i>
         *
         * @example
         * @code
         * if (FW::isKeyPressed(FW_KEY_Q)) {
         *     std::cout << "Key Q was just pressed" << std::endl;
         * }
         *
         * @param keycode Key to check against.
         * @return True if the key was just pressed. False if not.
         */
        static bool isKeyPressed(int keycode);

        /**
         * Checks whether the key was just pressed.
         *
         * @details It's recommended to use the key codes from
         * <i><u>KeyCodes.h</u></i>
         *
         * @example
         * @code
         * if (FW::isKeyJustPressed(FW_KEY_Q)) {
         *     std::cout << "Key Q was just pressed on this frame" << std::endl;
         * }
         *
         * @param keycode Key to check against
         * @return True if key was just pressed. If it is held down repeatedly
         * or released, return false.
         */
        static bool isKeyJustPressed(int keycode);

        /**
         * Check whether a certain key combination is pressed.
         *
         * @example
         * @code
         * if (FW::Input::isKeyCombinationPressed(FW_KEY_Q | FW_KEY_A)) {
         *     std::cout << "Q and A are pressed" << std::endl;
         * }
         * @endcode
         * @param keys Bits for the keys to check against.
         * @return True if the key combination is pressed. False if not.
         */
        static bool isModKeyCombinationPressed(FW_KEY_BIT keys);

        /**
         * Checks whether the mouse button was just pressed
         *
         * @details It's recommended to use the key codes from
         * <i><u>KeyCodes.h</u></i>
         *
         * @example
         * @code
         * if (FW::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
         *     std::cout << "Mouse left button was just pressed" << std::endl;
         * }
         *
         * @param button Mouse button to check against
         * @return True if the mouse button was just pressed, otherwise false.
         * If it's continuously held down, then also return false.
         */
        static bool isMouseButtonPressed(int mouseButton);

        /**
         * Get the mouse position in screen space.
         */
        static glm::vec2 getMousePosition();

        /**
         * Get the mouse x-position in screen space.
         */
        static float getMouseX();

        /**
         * Get the mouse y-position in screen space.
         */
        static float getMouseY();

        /**
         * Set the window to get user inputs from.
         *
         * @details Based on the inputs and the currently registered inputs,
         * different values will be returned. <ul> <li>0: Success</li> <li>1: A
         * window is already registered</li> <li>0: The window pointer passed as
         * argument is a nullptr.</li>
         * </ul>
         *
         * @details If class Input registers a new window during runtime, it's
         * recommended to check against these errors
         *
         * @param win The assigned window
         * @return Error codes
         */
        static bool setWindow(GLFWwindow* win);

        static int getCurrentKeyState() { return currentKeyState; }

    public:
        static KeyRegister registerKeyJustPressed;
        static MouseRegister registerMouseJustPressed;
        static GLFWwindow* window;
        static uint32_t currentKeyState;

        /** Disable in-game keyboard capture. Useful for capturing keyboard
         * events in UI elements.
         */
        static bool enableKeyboardCapture;
    };
}
