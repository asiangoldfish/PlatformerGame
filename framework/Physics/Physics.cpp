#include "Physics.h"

namespace FW {
    std::vector<BoundingBox_Quad*> isOverlapping(BoundingBox_Quad* bbox,
                                    std::vector<BoundingBox_Quad*>& otherBboxes)
    {
        std::vector<BoundingBox_Quad*> foundBboxes;

        for (auto& other : otherBboxes) {
            if (bbox != other && isOverlapping(*bbox, *other)) {
                foundBboxes.push_back(other);
            }
        }
        return foundBboxes;
    }

    bool isOverlapping(const BoundingBox_Quad& a,
                       const BoundingBox_Quad& b)
    {
        return
          a.minX <= b.maxX &&
          a.maxX >= b.minX &&
          a.minY <= b.maxY &&
          a.maxY >= b.minY &&
          a.minZ <= b.maxZ &&
          a.maxZ >= b.minZ;
    }
}
