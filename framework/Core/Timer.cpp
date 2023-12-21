#include "Timer.h"
#include <GLFW/glfw3.h>

namespace Framework {
	Timer::Timer() : lastFrame(0.f), deltaTime(0.f)
	{
	}

	void Timer::resetTimer()
	{
		auto currentFrame = (float)glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;
	}
}