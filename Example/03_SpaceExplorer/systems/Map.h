#pragma once

// Space Explorer libraries
#include "Ship.h"

// Framework
#include "Serializable.h"
#include "Math/glmTypes.h"

// External libraries
#include <glm/glm.hpp>

// Standard libraries
#include <filesystem>
#include <unordered_map>

class Chunk;
class ChunkData;

/**
 * The Map is the owner of the world and world generation.
 *
 * It manages chunks, dynamically loading and unloading chunks, and more.
 */
class MapSystem : public FW::SceneNode {
public:
    MapSystem() = default;
    virtual ~MapSystem() = default;

    void setPlayerShip(FW::ref<PlayerShip> ship) { playerShip = ship; }

    virtual void update(float delta) override;

    std::vector<Chunk>& getLoadedChunks() { return loadedChunks; }

private:
    /**
     * Load or unload a chunk based on the player's position.
     */
    void manageChunkLoading();
    void loadChunks();
    void unloadChunks();
    void generateChunk(glm::ivec2 newPos);

    /** Convert world position to chunk position */
    glm::ivec2 worldToChunk(const glm::vec2& pos);

private:
    /**
     * In-memory storage of chunk data.
     *
     * in <Math/glmTypes.h>, glm::ivec2 is overloaded. This means we can use the
     * position of a chunk as the hash/key/index for a particular ChunkData
     * object.
     */
    std::unordered_map<glm::ivec2, ChunkData> chunkCache;
    std::vector<Chunk> loadedChunks;

    std::weak_ptr<PlayerShip> playerShip;
    float chunkDistanceThreshold = 300.0f;
    float chunkSize = 1000.0f;
};

/**
 * The world is divided into chunks. A chunk is NxN size, and is loaded
 * dynamically by the Map.
 *
 * A chunk stores data like astroids.
 */
class Chunk : public FW::Serializable {
public:
    Chunk() = default;
    Chunk(glm::ivec2& position)
      : chunkPosition(position) {}
    Chunk(ChunkData& data) {}
    Chunk(std::filesystem::path filepath) {}

    virtual ~Chunk() = default;

    virtual void serialize() override { glm::vec2 v = glm::vec2(3, 3); }
    virtual void deserialize() override {}

public:
    glm::ivec2 chunkPosition{ 0 };
    glm::vec2 worldPosition{ 0.0f };

private:
    // Draw borders around the chunks for debugging
    bool debug = false;
};

/**
 * Temporary data storage for Chunks. These are data that should be persistant.
 * Consider this class as a DTO.
 */
struct ChunkData {
    glm::ivec2 chunkPosition;
    // std::vector<> meteors;
};