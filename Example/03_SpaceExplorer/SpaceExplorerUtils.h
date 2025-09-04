#pragma once

#include <glm/glm.hpp>

#include "Entity.h"
#include "Component.h"
#include "Util.h"
#include "Camera/Camera.h"

/** Returns angle in radian in z-axis */
inline glm::vec3 getRotationWithMouse() {
    double x = static_cast<double>(FW::Input::getMouseX());
    double xRatioWithScreen = (x / 640.0) - 1.0;
    double y = static_cast<double>(FW::Input::getMouseY());
    double yRatioWithScreen = (y / 360.0) - 1.0;

    return glm::vec3{ 0.f, 0.f, atan2(yRatioWithScreen, xRatioWithScreen) };
}

/**
 * Rotate a ship toward another
 * 
 * @param src the ship to rotate
 * @param dest the target ship to rotate toward
 */
inline float angleToEnemy(Ship* src, Ship* dest) {
    glm::vec2 d = dest->getPosition() - src->getPosition();
    return atan2(d.y, d.x);
}

/** Checks if mouse is inside an entity's bounds */
inline bool isMouseInsideEntityBounds(FW::ref<FW::SceneNode> node,
                                      const FW::ref<FW::Camera> camera) {
    // TODO consider the entity's rotation
    // Assume the entity never rotates. We know what the min is by finding the
    // position - entity size, and the max is position + entity size. It gives
    // us the following:
    // clang-format off
    // +--------X
    // |        |
    // |        |
    // X--------+
    // clang-format on
    // The X's are where the points we find are.

    auto transformation =
      node->entity->getComponent<FW::TransformationComponent>();
    glm::vec2 min =
      transformation->getPosition2D() - transformation->getScale2D();
    glm::vec2 max =
      transformation->getPosition2D() + transformation->getScale2D();

    // Condition A: Mouse pos.x > left border
    // Condition B: Mouse pos.x < right border
    // Condition C: Mouse pos.y > bottom border
    // Condition D: Mouse pos.y < top border
    glm::vec2 mousePos = FW::mouseToWorld2D(FW::Input::getMouseX(),
                                            FW::Input::getMouseY(),
                                            1280.0f,
                                            720.0f,
                                            camera->getViewMatrix(),
                                            camera->getProjectionMatrix());

    // clang-format off
    return (
        mousePos.x > min.x && // Condition A
        mousePos.x < max.x && // Condition B
        mousePos.y > min.y && // Condition C
        mousePos.y < max.y    // Condition D
    );
    // clang-format on
}