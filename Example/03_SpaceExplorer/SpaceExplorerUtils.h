#pragma once

#include <glm/glm.hpp>

/** Returns angle in radian in z-axis */
inline glm::vec3 getRotationWithMouse() {
    double x = static_cast<double>(FW::Input::getMouseX());
    double xRatioWithScreen = (x / 640.0) - 1.0;
    double y = static_cast<double>(FW::Input::getMouseY());
    double yRatioWithScreen = (y / 360.0) - 1.0;

    return glm::vec3{ 0.f, 0.f, atan2(yRatioWithScreen, xRatioWithScreen) };
}