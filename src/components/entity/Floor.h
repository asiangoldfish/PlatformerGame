#ifndef LAB_FLOOR_H
#define LAB_FLOOR_H

// External
#include <glm/glm.hpp>

// Framework
#include "Framework.h"

class Floor : public Framework::Entity
{
public:
    Floor() = default;
    Floor(Framework::ref<Framework::Shader> shader, glm::vec2 tiling, glm::vec2 size)
    {
        auto vertices = GeometricTools::UnitGridGeometry2D(
          tiling.x, tiling.y, size.x, size.y);

        auto indices = GeometricTools::UnitGridIndices2D(tiling.x * tiling.y);
        initDrawable(shader, vertices, indices);
    }

    virtual ~Floor() = default;

    /**
     * Divide the floor into tiles, and alternatively generate black and white
     * tiles.
     * @param x Tiles in x direction
     * @param y Tiles in y direction
     */
    void setTiles(uint32_t x, uint32_t y);

    /**
     * Generate colors for chessboard
     */
    void generateColors();

    /**
     * @brief Move green tile
     */
    void moveSelector(int x, int y);

    glm::vec2 getSelectorPosition() { return selectorPosition; }

private:
    int numOfQuads = 8;
    int tilesX, tilesY;
    glm::vec2 selectorPosition{ 0, 0 };
};

#endif // LAB_FLOOR_H
