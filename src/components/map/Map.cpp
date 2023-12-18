#include <fstream>
#include <iostream>
#include <string>

#include "Map.h"
#include "Cube.h"
#include "Shader.h"

// Constructor
Map::Map(Framework::Shader* shader)
{
    this->shader = shader;

    // This node is empty. All it does is containing other child nodes.
    baseNode = new Entity();
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
Map::draw()
{
    if (baseNode) {
        baseNode->draw();
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
        int index[2] = { 0 };

        while (getline(infile, readLine)) {
            for (auto& c : readLine) {
                switch (toupper(c)) {
                    case 'W':
                        Cube* wall = new Cube(shader);
                        wall->setScale(1.0f);
                        wall->setColor(glm::vec4{ 1.0f });
                        wall->setPosition({ index[0], index[1], 0});
                        wall->setTextureName("wall");
                        baseNode->addChild(wall);
                        break;
                }
                index[0]++;  // Increase X-position
            }
            index[0] = 0;   // Reset X-position as we are on a new line now
            index[1]--;     // Increase Y-position
        }

        // Close the file as we have finished reading it
        infile.close();
    } else {
        std::cout << "Map at \'" << mapPath << "\' cannot be found!"
                  << std::endl;
    }

    return true;
}
