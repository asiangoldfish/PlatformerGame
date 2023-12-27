#include <fstream>
#include <iostream>
#include <string>

#include "Map.h"
#include "Entity.h"
#include "Cube.h"
#include "Shader.h"

static Cube*
spawnWall(glm::vec3 pos, uint32_t diffTexId, FW::Entity* parent)
{
    Cube* cube = new Cube();
    cube->setPosition(pos);
    cube->getMaterial().getProperties().diffuseTextureId = diffTexId;
    cube->setParent(parent);
    return cube;
}

// Constructor
Map::Map()
{
    // This node is empty. All it does is containing other child nodes.
    baseNode = new FW::Entity();
}

// Delete associated entity nodes
Map::~Map()
{
    delete baseNode;
}

void
Map::update()
{
    if (baseNode) {
        baseNode->update();
    }
}

void
Map::draw(const FW::ref<FW::Shader>& shader)
{
    if (baseNode) {
        baseNode->draw(shader);
    }
}

// Add a new map
void
Map::addNewMap(const std::string& name, const std::string& path)
{
    // Only add if it does not exist
    auto mapIterator = mapsCollection.find(name);
    if (mapIterator == mapsCollection.end()) {
        // No existing maps found, so we add it
        mapsCollection[name] = path;
    }
}

// Load the map
bool
Map::loadMap(const std::string& name)
{
    std::string mapPath;

    auto mapIterator = mapsCollection.find(name);
    if (mapIterator == mapsCollection.end()) {
        // No maps by this name was found
        return false;
    }

    std::ifstream infile(mapIterator->second, std::ios::in | std::ios::binary);

    if (infile) {
        // Line that is currently read from the file
        std::string readLine;

        uint32_t wallTextureID = FW::TextureManager::getTextureID("wall");

        std::vector<std::string> linesFromFile;
        while (getline(infile, readLine)) {
            linesFromFile.push_back(readLine);
        }

        // Loop over each line in the file, from end to beginning.
        for (int y = linesFromFile.size() - 1; y > 0; y--) {
            // The map file might end with an empty line. Take this into account
            if (y == linesFromFile.size() - 1 && linesFromFile[y].empty()) {
                continue;
            }

            // Loop over each character in the given line
            for (int x = 0; x < linesFromFile[y].size(); x++) {
                std::string currentLine = linesFromFile[y];
                switch (toupper(currentLine[x])) {
                    case 'W':
                        baseNode->addChild(
                          spawnWall({ x, -y, 0 }, wallTextureID, baseNode));
                        baseNode->addChild(
                          spawnWall({ x, -y, 1 }, wallTextureID, baseNode));
                        break;
                }
            }
        }

        // Close the file as we have finished reading it
        infile.close();
    } else {
        std::cout << "Map at \'" << mapPath << "\' cannot be found!"
                  << std::endl;
    }

    return true;
}
