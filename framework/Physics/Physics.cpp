#include "Physics.h"

namespace Framework {
    /*
    void update(Entity* entity, uint32_t n)
    {
        for (int i = 0; i < n; i++) {
            update(entity[i]);
        }
    }

    void update(Entity& entity)
    {
        // Set all to undesirablest values.
        entity.getBoundingBox()->south = entity.getBoundingBox()->bot =
          entity.getBoundingBox()->west = HUGE_VALF;
        entity.getBoundingBox()->north = entity.getBoundingBox()->top =
          entity.getBoundingBox()->east = -HUGE_VALF;

        // Then find max and mins for each. Speedy and spimple.
        for (auto e : entity.getVertices()) {
            if (e.x > entity.getBoundingBox()->north) {
                entity.getBoundingBox()->north = e.x;
            }
            if (e.x < entity.getBoundingBox()->south) {
                entity.getBoundingBox()->south = e.x;
            }

            if (e.y > entity.getBoundingBox()->top) {
                entity.getBoundingBox()->top = e.y;
            }
            if (e.y < entity.getBoundingBox()->bot) {
                entity.getBoundingBox()->bot = e.y;
            }

            if (e.z > entity.getBoundingBox()->east) {
                entity.getBoundingBox()->east = e.z;
            }
            if (e.z < entity.getBoundingBox()->west) {
                entity.getBoundingBox()->west = e.z;
            }
        }
    }

    bool isOverlapping(Entity& target, Entity* allEntities, uint32_t n)
    {
        for (int i = 0; i < n; i++) {
            if (&target !=
                &allEntities[i]) { // We do not collide with ourselves
                if (isOverlapping(target, allEntities[i])) {
                    return true;
                }
            }
        }

        return false;
    }

    bool isOverlapping(Entity& entityA, Entity& entityB)
    {
        return (
          entityA.getBoundingBox()->north > entityB.getBoundingBox()->south &&
          entityA.getBoundingBox()->south < entityB.getBoundingBox()->north &&

          entityA.getBoundingBox()->top > entityB.getBoundingBox()->bot &&
          entityA.getBoundingBox()->bot < entityB.getBoundingBox()->top &&

          entityA.getBoundingBox()->east > entityB.getBoundingBox()->west &&
          entityA.getBoundingBox()->west < entityB.getBoundingBox()->east);
    }
     */
} // Framework
