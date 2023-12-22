#include "Camera.h"
#include <cmath>

#include "glm/gtc/matrix_transform.hpp"
#include <iostream>

namespace Framework {
	void Camera::moveForward(const float speed)
	{
		// From ChatGPT
		position += speed * cameraFront;
		computeViewMatrix();
	}

	void Camera::moveSideway(const float speed)
	{
		// From ChatGPT
		position += 
			glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
		computeViewMatrix();
	}

	void Camera::moveUp(const float speed) {
		position.y += speed;
		computeViewMatrix();
	}

	void Camera::rotate(const glm::vec2& rotation)
	{
		// From ChatGPT
		yaw += rotation.x;

		// Clamp so the camera doesn't go upside down
		if (pitch - rotation.y >= -90 && pitch - rotation.y <= 90) {
			pitch -= rotation.y;
		}
		computeViewMatrix();
	}
}
