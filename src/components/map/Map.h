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
    explicit Map(Framework::Shader* shader);

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

    /** Get the map's height */
    int getHeight() const { return height; }

    /** Get the map's width */
    int getWidth() const { return width; }

    [[nodiscard]] Entity* getPlayer() const { return player; }

private:
    /** <mapName, filepath> */
    std::map<std::string, std::string> mapsCollection;
    Entity* baseNode = nullptr;
    Entity* player = nullptr;

    Framework::Shader* shader = nullptr;

    int height = 0;  ///< Map size in height
    int width = 0;   ///< Map size in width
};

#endif // MAP_H
