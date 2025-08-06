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
 *
 * For now, the AssetSystem is a static class. This may be changed in the
 * future.
 */
class AssetSystem {
public:
    AssetSystem() = delete;

    static std::vector<Asset>& getAssets() { return assets; }

public: // IO
    /**
     * While the constructor already loads from disk upon instantiation, this
     * can manually be queries.
     */
    static void loadFromDisk();

    static void serializeAsset();

public:
    static std::vector<Asset> assets;

    static std::string filepath;

    /**
     * Test for spawning a cube.
     * If a cube should be spawned on the next frame, this is true.
     */
    static bool spawnCube;
};