#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace FW {
    struct BoundingBox_Quad
    {
        void setScale(float x, float y, float z) { scale = glm::vec3(x, y, z); }

        void setScale(float s) { scale = glm::vec3(s); }

        void setScale(glm::vec3 s) { scale = s; }

        void setPosition(glm::vec3 position)
        {
            maxX = position.x + scale.x;
            minX = position.x;
            maxY = position.y + scale.y;
            minY = position.y;
            maxZ = position.z + scale.z;
            minZ = position.z;
        }

        glm::vec3 scale{ 1.0f };
        float maxX = 0.0f, minX = 0.0f; // x-axis
        float maxY = 0.0f, minY = 0.0f; // y-axis
        float maxZ = 0.0f, minZ = 0.0f; // z-axis

        BoundingBox_Quad() { setPosition(glm::vec3(0.0f)); }
    };

    std::vector<BoundingBox_Quad*> isOverlapping(
      BoundingBox_Quad* bbox,
      std::vector<BoundingBox_Quad*>& otherBboxes);

    bool isOverlapping(const BoundingBox_Quad& a, const BoundingBox_Quad& b);
} // FW
