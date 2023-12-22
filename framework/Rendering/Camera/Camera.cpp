#include "Camera.h"

#include "glm/gtc/matrix_transform.hpp"
#include "pch.h"

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
                pitch += rotation.y;
                pitch = std::clamp(pitch, -89.9f, 89.9f);

		computeViewMatrix();
	}
}
