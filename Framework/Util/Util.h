#include <glm/glm.hpp>

namespace FW {
    /**
     * Limit a given point's positions to its bounds.
     *
     * @details If the point is outside the bounds, it is snapped to the bounds.
     * @details To determine whether the point is outside bounds, logical operations are performed. This means it works for both
     * screen space and world space. However, one of them must be chosen. If the point's position is given in screen
     * space, then the bounds must also be in screen space.
     *
     * @example
     * @code
     * /// If the mouse cursor is outside the window, it's snapped back to the window.
     * #include <glm/glm.hpp>
     *
     * // Get the window size. This is boundsMax.
     * glm::vec2 windowSize = getWindowSize();
     * // Get the mouse position
     * glm::vec2 mousePosition = getMousePosition();
     * // Compute the new mouse position after checking for bounds collision.
     * mousePosition = limitPointToBounds(mousePosition, glm::vec2(0.0f, 0.0f), windowSize());
     * @endcode
     *
     * @param pos A copy of the point's position.
     * @param boundsMin The bound's minimum position.
     * @param boundsMax The bound's maximum position.
     * @return The modified point's position. If no modifications were made, then the original position is returned.
     */
    glm::vec2 limitPointToBounds(glm::vec2 pos,
                                 glm::vec2 boundsMin,
                                 glm::vec2 boundsMax);

    glm::vec2 mouseToWorld2D(float mouseX,
                             float mouseY,
                             int windowWidth,
                             int windowHeight,
                             const glm::mat4& viewMatrix,
                             const glm::mat4& projectionMatrix);
}
