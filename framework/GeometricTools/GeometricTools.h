#pragma once

/**
 * The GeometricTools module provides geometries with attributes like position
 * and color.
 */

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define FW_VERTEX_ATTRIBUTES uint32_t
// Prevent compiler error
#ifndef KEY_BIT
#define KEY_BIT(key) (1UL << key)
#endif

namespace FW {
    enum class VERTEX_ATTRIBUTE : uint32_t {
        POSITION = KEY_BIT(0),
        COLOR = KEY_BIT(1),
        TEXTURE_COORD = KEY_BIT(2),
        VERTEX_NORMAL = KEY_BIT(3)
    };

    inline bool operator&(VERTEX_ATTRIBUTE lhs, VERTEX_ATTRIBUTE rhs) {
        return (static_cast<uint32_t>(lhs) & static_cast<uint32_t>(rhs)) != 0;
    }

    /**
     * Generate a 2-dimensional triangle with positions
     */
    constexpr std::array<float, 2 * 3> UnitTriangle2D = { 0.0f,  0.5f,  0.5f,
                                                          -0.5f, -0.5f, -0.5f };

    /**
     * Generate a 2-dimensional square with positions
     */
    inline std::vector<float> UnitSquareVertices2D = {
        // Triangle 1       // Corners:
        -1.0f, -1.0f, -1.0f, // A
        1.0f,  -1.0f, -1.0f, // B
        1.0f,  1.0f,  -1.0f, // C
        -1.0f, 1.0f,  -1.0f, // D
    };

    inline std::vector<float> UnitSpriteVertices = {
        // Triangle 1       // Corners:
        // Position            Colour                TexCoord        Normal
        // X    Y     Z     R      G     B    A      U    V       X    Y    Z
        -0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // A
        0.5f,  -0.5f, 0.0f, 1.0f, 1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // B
        0.5f,  0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // C
        -0.5f, 0.5f,  0.0f, 1.0f, 1.0f, 1.0f,
        1.0f,  1.0f,  1.0f, 1.0f, 1.0f, 1.0f, // D
    };

    inline std::vector<uint32_t> UnitGridIndices2D = {
        0, 1, 2, 2, 3, 0,
    };

    inline std::vector<float> UnitGridGeometry2D(
      VERTEX_ATTRIBUTE flags = VERTEX_ATTRIBUTE::POSITION) {
        float baseColor = 1.0f;
        std::array<float, 8> texCoords = {
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
        };

        std::vector<float> out;

        // Add attributes based on flags
        for (int i = 0; i < 4; i++) {
            if (flags & VERTEX_ATTRIBUTE::POSITION) {
                int offset = i * 3;
                out.push_back(UnitSquareVertices2D[0 + offset]);
                out.push_back(UnitSquareVertices2D[1 + offset]);
                out.push_back(UnitSquareVertices2D[2 + offset]);
            }

            if (flags & VERTEX_ATTRIBUTE::COLOR) {
                // Base color in R, G and B. 1.0f in A
                out.push_back(baseColor); // R
                out.push_back(baseColor); // G
                out.push_back(baseColor); // B
                out.push_back(1.0f);      // A
            }

            if (flags & VERTEX_ATTRIBUTE::TEXTURE_COORD) {
                int offset = i * 2;
                out.push_back(texCoords[0 + offset]);
                out.push_back(texCoords[1 + offset]);
            }

            if (flags & VERTEX_ATTRIBUTE::VERTEX_NORMAL) {
                out.push_back(0.0f);
                out.push_back(0.0f);
                out.push_back(1.0f);
            }
        }

        return out;
    }

    // TODO: Tile the checkerboard, but it still remains 1x1 size
    /*
     * TODO: Add enum class that lets the user choose what orientation to
     * generate the checkerboard in. Example: XY, YZ
     */
    /**
     * Generate a checkerboard.
     *
     * @param tilingX Tiling in x-direction
     * @param tilingY Tiling in y-direction
     * @return
     */
    inline std::vector<float> UnitCheckerboard2D(int tilingX, int tilingY) {
        float baseColor = 1.0f;

        // Tiling cannot be less than 0
        tilingX = tilingX < 0 ? 0 : tilingX;
        tilingY = tilingY < 0 ? 0 : tilingY;

        std::vector<float> baseGrid = {
            // clang-format off
            /*
             * Note: Normal is negative Z, as the grid is created in 2D space.
             * This means it's facing
             */
            // Position		    // Color                                  // Tex coords   // Normals
            -0.5f, -0.5f, 0.0f,     baseColor, baseColor, baseColor, 1.0f,    0, 0,           0, 0, 1,
            0.5f,  -0.5f, 0.0f,     baseColor, baseColor, baseColor, 1.0f,    1, 0,           0, 0, 1,
            0.5f,  0.5f,  0.0f,     baseColor, baseColor, baseColor, 1.0f,    1, 1,           0, 0, 1,
            -0.5f, 0.5f,  0.0f,     baseColor, baseColor, baseColor, 1.0f,    0, 1,           0, 0, 1
            // clang-format on
        };

        // Grid to return from this function. Because the vector's elements
        // are stored in heap, we can return a reference to the vector.
        std::vector<float> returnGrid;

        int numOfAttributes = 12;

        /*
         * Algorithm: For each row, append an additional baseGrid to returnGrid.
         * Repeat for each time we tile in the y-axis.
         */
        for (int y = 0; y < tilingY; y++) {
            for (int x = 0; x < tilingX; x++) {

                // Iterate every attribute in baseGrid
                for (int i = 0; i < baseGrid.size(); i++) {
                    int attributeIndex = i % (numOfAttributes);
                    if (attributeIndex == 0) {
                        returnGrid.push_back(baseGrid[i] + (float)x);
                    } else if (attributeIndex == 1) {
                        returnGrid.push_back(baseGrid[i] + (float)y);
                    } else {
                        returnGrid.push_back(baseGrid[i]);
                    }
                }
            }
        }

        /*
         * Return a reference to the new grid (the address of returnGrid is
         * a pointer to the first element in the datastructure).
         */
        return returnGrid;
    }

    /**
     * Generate the indices to a tiled grid.
     *
     * @param numOfQuads Total number of quads in the grid.
     * @return A vector with all the generated indices.
     * @see UnitCheckerboard2D()
     * @example If a grid has 3x3 cells, it has a total of 9 cells. This means
     * numOfQuads is also 9.
     */
    inline std::vector<uint32_t> UnitCheckerboardIndices2D(int numOfQuads) {
        // Base indices for one quad.
        std::vector<uint32_t> baseIndices = { 0, 1, 2, 2, 3, 0 };

        // Vector to return to the caller.
        std::vector<uint32_t> returnIndices;

        for (int i = 0; i < numOfQuads; i++) {
            for (const auto& index : baseIndices) {
                returnIndices.push_back(index + (i * 4));
            }
        }

        return returnIndices;
    }

    /**
     * Generate a unit cube.
     *
     * The following attributes are generated:
     * - Position (x, y, z)
     * - Color (r, g, b, a)
     * - UV - texture coordinates (u, v)
     *
     * @return Vertex attributes for a unit cube.
     */
    inline std::vector<float> UnitCubeGeometry3D() {
        float baseColor = 1.0f, alpha = 1.0f;
        std::vector<float> container = {
            // clang-format off
            // Position				// Color // UV  // Normals
            // Front
            -0.5f,	-0.5f, -0.5f,	baseColor, baseColor, baseColor, alpha, 0, 0,   0, 0, -1,
            0.5f,	-0.5f, -0.5f,	baseColor, baseColor, baseColor, alpha, 1, 0,   0, 0, -1,
            0.5f,	0.5f,  -0.5f,	baseColor, baseColor, baseColor, alpha, 1, 1,   0, 0, -1,
            -0.5f,	0.5f,  -0.5f,	baseColor, baseColor, baseColor, alpha, 0, 1,   0, 0, -1,

            // Rear
            -0.5f,	-0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 0, 0,   0, 0, 1,
            0.5f,	-0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 1, 0,   0, 0, 1,
            0.5f,	0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 1, 1,   0, 0, 1,
            -0.5f,	0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 0, 1,   0, 0, 1,

            // Top
            -0.5f,	0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 0, 0,   0, 1, 0,
            0.5f,	0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 1, 0,   0, 1, 0,
            0.5f,	0.5f,	 0.5f,	baseColor, baseColor, baseColor, alpha, 1, 1,   0, 1, 0,
            -0.5f,	0.5f,	 0.5f,	baseColor, baseColor, baseColor, alpha, 0, 1,   0, 1, 0,

            // Bottom
            -0.5f,      -0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 0, 0,   0, -1, 0,
            0.5f,	-0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 1, 0,   0, -1, 0,
            0.5f,	-0.5f,	 0.5f,	baseColor, baseColor, baseColor, alpha, 1, 1,   0, -1, 0,
            -0.5f,      -0.5f,	 0.5f,	baseColor, baseColor, baseColor, alpha, 0, 1,   0, -1, 0,

            // Right
            -0.5f,	-0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 0, 0,   -1, 0, 0,
            -0.5f,	0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 1, 0,   -1, 0, 0,
            -0.5f,	0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 1, 1,   -1, 0, 0,
            -0.5f,	-0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 0, 1,   -1, 0, 0,

            // Left
            0.5f,	-0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 0, 0,   1, 0, 0,
            0.5f,	0.5f,	-0.5f,	baseColor, baseColor, baseColor, alpha, 1, 0,   1, 0, 0,
            0.5f,	0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 1, 1,   1, 0, 0,
            0.5f,	-0.5f,	0.5f,	baseColor, baseColor, baseColor, alpha, 0, 1,   1, 0, 0,
            // clang-format on
        };
        return container;
    }

    /**
     * Generate the indices for a unit cube.
     * @return Indices for a unit cube.
     */
    inline std::vector<unsigned int> UnitCubeGeometry3DIndices() {
        std::vector<unsigned int> container;

        // To support culling, we must invert the order every other iteration
        std::vector<unsigned int> indices = { 0, 1, 2, 2, 3, 0 };
        std::vector<unsigned int> invertedIndices = { 0, 2, 1, 2, 0, 3 };

        int vertices = 4; // Number of vertices in a quad
        bool isFrontFacing = true;
        for (int i = 0; i < 6; i++) {
            // Select between normal or inverted indices
            std::vector<unsigned int> ptr;
            if ((isFrontFacing = !isFrontFacing)) {
                ptr = indices;
            } else {
                ptr = invertedIndices;
            }

            // Append indices to container
            for (const auto& t : ptr) {
                container.push_back(vertices * i + t);
            }
        }

        return container;
    }

    /**
     * Generate skybox vertices
     *
     * The following attributes are generated:
     * - Position (x, y, z)
     *
     * To generate the indices, use SkyboxGeometryIndices()
     *
     * @return Position attributes
     */
    inline std::vector<float> SkyboxGeometryVertices() {
        return {
            // clang-format off
            // Front
            -0.5f,	-0.5f, -0.5f,
            0.5f,	-0.5f, -0.5f,
            0.5f,	0.5f,  -0.5f,
            -0.5f,	0.5f,  -0.5f,

            // Rear
            -0.5f,	-0.5f,	0.5f,
            0.5f,	-0.5f,	0.5f,
            0.5f,	0.5f,	0.5f,
            -0.5f,	0.5f,	0.5f,

            // Top
            -0.5f,	0.5f,	-0.5f,
            0.5f,	0.5f,	-0.5f,
            0.5f,	0.5f,	 0.5f,
            -0.5f,	0.5f,	 0.5f,

            // Bottom
            -0.5f,      -0.5f,	-0.5f,
            0.5f,	-0.5f,	-0.5f,
            0.5f,	-0.5f,	 0.5f,
            -0.5f,      -0.5f,	 0.5f,

            // Right
            -0.5f,	-0.5f,	-0.5f,
            -0.5f,	0.5f,	-0.5f,
            -0.5f,	0.5f,	0.5f,
            -0.5f,	-0.5f,	0.5f,

            // Left
            0.5f,	-0.5f,	-0.5f,
            0.5f,	0.5f,	-0.5f,
            0.5f,	0.5f,	0.5f,
            0.5f,	-0.5f,	0.5f,
            // clang-format on
        };
    }

    inline std::vector<unsigned int> SkyboxGeometryIndices() {
        return UnitCubeGeometry3DIndices();
    }
} // FW
