#pragma once

/**
 * The GeometricTools module provides geometries with attributes like position
 * and color.
 */

#include <array>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace GeometricTools {
    /**
     * Generate a 2-dimensional triangle with positions
     */
    constexpr std::array<float, 2 * 3> UnitTriangle2D = { 0.0f,  0.5f,  0.5f,
                                                          -0.5f, -0.5f, -0.5f };

    /**
     * Generate a 2-dimensional square with positions
     */
    constexpr std::array<float, 2 * 6> UnitSquare2D = { -0.25f, -0.25f, 0.25f,
                                                        -0.25f, -0.25f, 0.25f,

                                                        0.25f,  -0.25f, -0.25f,
                                                        0.25f,  0.25f,  0.25f };

    /**
     * Generate a unit grid
     *
     * By leaving tiling to 1, the geometry will be a unit grid. For each,
     * additional grids will be appended to the output vector, effectively
     * increasing the final grid additively. The size parameters will increase
     * the overall grid size.
     *
     * @param tilingX Tile additional grids in the x-axis
     * @param tilingY Tile additional grids in the y-axis
     * @param sizeX Increase the grid's size in the x-axis
     * @param sizeY Increase the grid's size in the y-axis
     * @return
     */
    inline std::vector<float> UnitGridGeometry2D(int tilingX,
                                                 int tilingY,
                                                 float sizeX = 1.0f,
                                                 float sizeY = 1.0f)
    {
        float baseColor = 1.0f;

        // Must be at least 1 repetition
        if (tilingX <= 0 || tilingY <= 0) {
            tilingX = 1;
            tilingY = 1;
        }

        std::vector<float> baseGrid = {
            /*
             * Note: Normal is negative Z, as the grid is created in 2D space.
             * This means it's facing
             */
            // Position				// Color
            // // Tex coords   // Normals
            -0.5f, -0.5f, 0.0f, baseColor, baseColor, baseColor,
            1.0f,  0,     0,    0,         0,         1,
            0.5f,  -0.5f, 0.0f, baseColor, baseColor, baseColor,
            1.0f,  1,     0,    0,         0,         1,
            0.5f,  0.5f,  0.0f, baseColor, baseColor, baseColor,
            1.0f,  1,     1,    0,         0,         1,
            -0.5f, 0.5f,  0.0f, baseColor, baseColor, baseColor,
            1.0f,  0,     1,    0,         0,         1
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
                        returnGrid.push_back((baseGrid[i] + (float)x) * sizeX);
                    } else if (attributeIndex == 1) {
                        returnGrid.push_back((baseGrid[i] + (float)y) * sizeY);
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
     * @see UnitGridGeometry2D()
     * @example If a grid has 3x3 cells, it has a total of 9 cells. This means
     * numOfQuads is also 9.
     */
    inline std::vector<uint32_t> UnitGridIndices2D(int numOfQuads)
    {
        // Base indices for one quad.
        std::vector<uint32_t> baseIndices = { 0, 1, 3, 1, 3, 2 };

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
     * Shorthand for UnitGridIndices2D(1).
     *
     * @return Indices for a quad.
     * @see UnitGridIndices2D(...)
     */
    inline std::vector<uint32_t> UnitGridIndices2D()
    {
        return UnitGridIndices2D(1);
    }

    /**
     * Shorthand for UnitGridGeometry(1, 1).
     *
     * @return Vertex attributes for a unit grid.
     */
    inline std::vector<float> UnitGridGeometry2D()
    {
        return UnitGridGeometry2D(1, 1);
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
    inline std::vector<float> UnitCubeGeometry3D()
    {
        float baseColor = 1.0f, alpha = 1.0f;
        std::vector<float> container = {
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
        };
        return container;
    }

    /**
     * Generate the indices for a unit cube.
     * @return Indices for a unit cube.
     */
    inline std::vector<unsigned int> UnitCubeGeometry3DIndices()
    {
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
    inline std::vector<float> SkyboxGeometryVertices()
    {
        return {
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
        };
    }

    inline std::vector<unsigned int> SkyboxGeometryIndices()
    {
        return UnitCubeGeometry3DIndices();
    }
}
