#include <fstream>
#include <iostream>
#include <string>

#include "Map.h"
#include "Entity.h"
#include "Cube.h"
#include "Shader.h"

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
        uint32_t coralStoneTextureID =
          FW::TextureManager::getTextureID("coral_stone_diff");

        std::vector<std::string> linesFromFile;
        while (getline(infile, readLine)) {
            linesFromFile.push_back(readLine);
        }

        // Loop over each line in the file, from end to beginning.
        for (int y = linesFromFile.size() - 1; y > 0; y--) {
            // The map file might end with an empty line. Take this into account
            if (y == linesFromFile.size() - 1 && linesFromFile[y].empty() ) {
                continue;
            }

            Cube* entity = nullptr;
            bool wallFound = false;

            // Loop over each character in the given line
            for (int x = 0; x < linesFromFile[y].size(); x++) {
                std::string currentLine = linesFromFile[y];
                switch (toupper(currentLine[x])) {
                    case 'W':
                        entity = new Cube();
                        entity->setPosition({ x, -y, 0 });
                        entity->getMaterial().getProperties().diffuseTextureId =
                          wallTextureID;
                        entity->setParent(baseNode);
                        baseNode->addChild(entity);

                        entity = new Cube();
                        entity->setPosition({ x, -y, 1 });
                        entity->getMaterial().getProperties().diffuseTextureId =
                          wallTextureID;
                        entity->setParent(baseNode);
                        baseNode->addChild(entity);

                        wallFound = true;
                        break;

                        /*
                    case ' ':
                        // Only spawn background if we have first spawned a
                        // wall.
                        if (wallFound) {
                            entity = new Cube();
                            entity->setPosition({ index, height, -1 });
                            entity->getMaterial()
                              .getProperties().diffuseTextureId =
                        coralStoneTextureID; entity->setParent(baseNode);
                            baseNode->addChild(entity);
                        }
                        break;
*/
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
