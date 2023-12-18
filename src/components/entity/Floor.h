#ifndef LAB_FLOOR_H
#define LAB_FLOOR_H

#include "Entity.h"
#include "GeometricTools.h"

class Floor : public Entity
{
public:
    Floor() = default;
	Floor(Framework::Shader* shader,
		std::vector<float> vertices,
		std::vector<uint32_t> indices,
		float scale = 1.0f
	)
		: Entity(shader, vertices, indices, GL_DYNAMIC_DRAW)
		, tilesX(1)
		, tilesY(1)
	{
		this->scale = glm::vec3(scale);
	}

    virtual ~Floor() = default;

    /**
     * Divide the floor into tiles, and alternatively generate black and white tiles.
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

    glm::vec2 getSelectorPosition() {
        return selectorPosition;
    }

private:
	int numOfQuads = 8;
	int tilesX, tilesY;
	glm::vec2 selectorPosition{ 0, 0 };
};

#endif // LAB_FLOOR_H