#include <glm/gtc/matrix_transform.hpp>

namespace FW {
    glm::vec2 limitPointToBounds(glm::vec2 pos,
                                 glm::vec2 boundsMin,
                                 glm::vec2 boundsMax) {
        // Position X
        if (pos.x < boundsMin.x) {
            pos.x = 0;
        } else if (pos.x > boundsMax.x) {
            pos.x = boundsMax.x;
        }

        // Position Y
        if (pos.y < boundsMin.x) {
            pos.y = 0;
        } else if (pos.y > boundsMax.y) {
            pos.y = boundsMax.y;
        }

        return pos;
    }

    glm::vec2 mouseToWorld2D(float mouseX,
                             float mouseY,
                             int windowWidth,
                             int windowHeight,
                             const glm::mat4& viewMatrix,
                             const glm::mat4& projectionMatrix) {
        // OpenGL expects origin bottom-left, so flip Y
        glm::vec3 screen(
          mouseX, windowHeight - mouseY, 0.0f);

        // Define viewport rectangle (x, y, width, height)
        glm::vec4 viewport(0.0f, 0.0f, windowWidth, windowHeight);

        // Unproject at depth = 0.0 (near plane)
        glm::vec3 world =
          glm::unProject(screen, viewMatrix, projectionMatrix, viewport);

        return glm::vec2{ world.x,
                          world.y }; // contains (x,y) in game coordinates
    }

}
