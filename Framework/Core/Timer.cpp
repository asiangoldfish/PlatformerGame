#include "Timer.h"

#include <GLFW/glfw3.h>

namespace FW {
    void Timer::updateDeltaTime()
    {
        auto currentTime = static_cast<float>(glfwGetTime());
        deltaTime =  currentTime - lastFrame;
        lastFrame = currentTime;
    }
} // Framework
