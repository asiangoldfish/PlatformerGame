//
// Created by khai on 18/12/23.
//

#ifndef MAP_H
#define MAP_H

#include <map>
#include <string>
#include <vector>

namespace Framework {
    class Shader;
}

class Entity;

class Map
{
public:
    /**
     * Default constructor
     */
    Map(Framework::Shader* shader);

    /**
     * Delete all entities belonging to this map.
     */
    virtual ~Map();

    /**
     * Update the entity tree structure.
     *
     * This function should be called once per frame.
     */
    void update();

    /**
     * Draw the entity tree
     */
    void draw();

    /**
     * Adds a new map. This can be loaded with loadMap()
     */
    void addNewMap(const std::string& name, const std::string& path);

    /** Load a map by its name */
    bool loadMap(const std::string& name);

private:
    /** <mapName, filepath> */
    std::map<std::string, std::string> mapsCollection;
    Entity* baseNode = nullptr;
    Framework::Shader* shader;
};

#endif // MAP_H
