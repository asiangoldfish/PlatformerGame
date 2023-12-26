#include "Timer.h"

#include <GLFW/glfw3.h>

namespace FW {
    void Timer::updateDeltaTime()
    {
        auto currentTime = static_cast<float>(glfwGetTime()) / 1000.0f;
        deltaTime =  lastFrame;
        lastFrame = currentTime;
    }
} // Framework
