//
// Created by khai on 13/12/23.
//

#ifndef PHYSICS_H
#define PHYSICS_H

#include <iostream>
#include <cstdint>

class Entity;

namespace Physics {
    struct BoundingBox {
        float north, south, top, bot, east, west;

        void print(); // Print the bounding box.
        /* We could also just overload the << operator
          Source: https://learn.microsoft.com/en-us/cpp/standard-library/overloading-the-output-operator-for-your-own-classes?view=msvc-170 */
//        friend ostream& operator<<(ostream& os, const BoundingBox& bb);
    };

    void update(Entity* entity, uint32_t n);  // Update BoundingBox with AABB
    void update(Entity& entity);  // Update BoundingBox with AABB
    bool isOverlapping(Entity& target, Entity* allEntities, uint32_t n);
    bool isOverlapping(Entity& entityA, Entity& entityB);

    /* Some more helper functions */
}

#endif // PHYSICS_H
