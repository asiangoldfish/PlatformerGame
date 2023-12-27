#include "Floor.h"

#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"

void Floor::setTiles(uint32_t x, uint32_t y)
{
    tilesX = x;
    tilesY = y;
//    generateColors();
}

void Floor::moveSelector(int x, int y)
{
    int moveToPositionX = selectorPosition.x + x;
    int moveToPositionY = selectorPosition.y + y;

    // Check bounds
    if (moveToPositionX >= 0 && moveToPositionX < tilesX &&
        moveToPositionY >= 0 && moveToPositionY < tilesY) {
        selectorPosition.x = moveToPositionX;
        selectorPosition.y = moveToPositionY;
    }

    generateColors();
}

void Floor::generateColors() {
    std::vector<std::vector<float>> color;
    bool offsetBlack = true;
    
    // Set white/black tiling color
    float white = 1.0f;
    float black = 0.25f;

    // Generate black and white
    for (int y = 0; y < tilesY; y++) {
        offsetBlack = !offsetBlack;
        color.push_back(std::vector<float>());
        for (int x = 0; x < tilesX; x++) {
            // If offsetBlack is true, then every even x is black
            if (offsetBlack) {
                color[y].push_back(x % 2 == 0);
            } else {
                color[y].push_back(x % 2 != 0);
            }

            // Set black or white
            if (color[y].back()) { // white
                color[y].back() = white;
            } else { // black
                color[y].back() = black;
            }
        }
    }

    /*
    Positions: 3
    Colors: 4

    Stride: 7
    Offset (for color): 3 * sizeof(float)
    Size: 4 * sizeof(float)
    */
    // Set color attributes to get chessboard tile colors
    int numOfAttributes = 12;
    int colorOffset = 3 * sizeof(float);

    vertexArray->bind();

    // The attribute index per vertex
    for (int y = 0; y < tilesY; y++) {
        for (int x = 0; x < tilesX; x++) {

            // Iterate every attribute in container
            for (int i = 0; i < 4; i++) {
                float data[3];
                if (x == selectorPosition.x && y == selectorPosition.y) {
                    data[0] = 0.0f;
                    data[1] = 1.0f;
                    data[2] = 0.0f;
                } else {
                    data[0] = color[y][x];
                    data[1] = color[y][x];
                    data[2] = color[y][x];
                }

                // Offset beginning of color attribute
                int offset = colorOffset + numOfAttributes * i * sizeof(float);
                // Add offset for each square in x-axis
                offset += x * numOfAttributes * 4 * sizeof(float);
                // Add offset for each row in y-axis
                offset += y * numOfAttributes * 4 * sizeof(float) * tilesX;
                vertexBuffer->bufferSubData(offset, sizeof data, data);
            }
        }
    }
}
