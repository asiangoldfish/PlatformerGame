#pragma once

#include "Asset.h"

#include <vector>
#include <filesystem>

class PhysicsScene;
class AppWidget;

/**
 * The Filesystem is a collection of states and queries. This is useful for
 * transferring data between components that otherwise don't know about each
 * other, like the AppWidget and the PhysicsScene.
 */
class AssetSystem {
public:
    AssetSystem() = default;
    virtual ~AssetSystem() = default;

    std::vector<Asset>& getAssets() { return assets; }

public: // IO
    /**
     * While the constructor already loads from disk upon instantiation, this
     * can manually be queries.
     */
    void loadFromDisk();

    void serializeAsset();

private:
    friend PhysicsScene;
    friend AppWidget;

    std::vector<Asset> assets;

    std::string filepath;

    /**
     * Test for spawning a cube.
     * If a cube should be spawned on the next frame, this is true.
     */
    bool spawnCube = false;
};