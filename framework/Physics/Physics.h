#ifndef PHYSICS_H
#define PHYSICS_H

//#include "pch.h"
//#include "Entity.h"

namespace Framework {
    struct BoundingBox
    {
        float north, south, top, bot, east, west;
    };

//    void update(Entity* entity, uint32_t n); // Update BoundingBox with AABB
//    void update(Entity& entity);             // Update BoundingBox with AABB
//    bool isOverlapping(Entity& target, Entity* allEntities, uint32_t n);
//    bool isOverlapping(Entity& entityA, Entity& entityB);
} // Framework

#endif // PHYSICS_H
