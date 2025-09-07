#include "Map.h"

void MapSystem::update(float delta) {
    FW::SceneNode::update(delta);

    manageChunkLoading();
}

void MapSystem::manageChunkLoading() {
    unloadChunks();
    loadChunks();
}

void MapSystem::loadChunks() {
    // Algorithm:
    // 1. Find the player position in chunk space
    // 2. Load all chunks 1 unit around the player's chunk space
    //
    // If P is the player's chunk and [] is the chunk to load:
    // []  []  []
    // []  P   []
    // []  []  []

    auto playerRef = playerShip.lock();

    if (!playerRef) {
        return;
    }

    glm::ivec2 P = worldToChunk(playerRef->getPosition());

    // Order: Top left to bottom right.
    //
    // clang-format off
    std::array<glm::ivec2, 9> chunkPositions {
        // 1st row
        glm::ivec2(P.x - 1, P.y - 1),
        glm::ivec2(P.x,     P.y - 1),
        glm::ivec2(P.x + 1, P.y - 1),

        // 2nd row
        glm::ivec2(P.x - 1, P.y    ),
        glm::ivec2(P.x    , P.y    ),
        glm::ivec2(P.x + 1, P.y    ),

        // 3rd row
        glm::ivec2(P.x - 1, P.y + 1),
        glm::ivec2(P.x,     P.y + 1),
        glm::ivec2(P.x + 1, P.y + 1),
    };
    // clang-format on

    for (const auto& pos : chunkPositions) {
        // Find the chunk among loaded chunks
        bool isAlreadyLoaded = false;
        bool isCached = false;
        for (const auto& chunk : loadedChunks) {
            if (chunk.chunkPosition == pos) {
                isAlreadyLoaded = true;
                break;
            }
        }

        if (!isAlreadyLoaded) {
            // Chunk is not already loaded, so try to find it in cache.
            auto iter = chunkCache.find(pos);
            if (iter != chunkCache.end()) {
                loadedChunks.emplace_back(iter->second);
                isCached = true;
            } else {
                generateChunk(pos);
            }
        }
    }
}

void MapSystem::unloadChunks() {
    // Algorithm:
    // 1. Find the player position in chunk space
    // 2. Load all chunks 1 unit around the player's chunk space
    //
    // If P is the player's chunk and [] is the chunk to load:
    // []  []  []
    // []  P   []
    // []  []  []

    auto playerRef = playerShip.lock();

    if (!playerRef) {
        return;
    }

    glm::ivec2 P = worldToChunk(playerRef->getPosition());

    // Order: Top left to bottom right.
    //
    // clang-format off
    std::array<glm::ivec2, 9> chunkPositions {
        // 1st row
        glm::ivec2(P.x - 1, P.y - 1),
        glm::ivec2(P.x,     P.y - 1),
        glm::ivec2(P.x + 1, P.y - 1),

        // 2nd row
        glm::ivec2(P.x - 1, P.y    ),
        glm::ivec2(P.x    , P.y    ),
        glm::ivec2(P.x + 1, P.y    ),

        // 3rd row
        glm::ivec2(P.x - 1, P.y + 1),
        glm::ivec2(P.x,     P.y + 1),
        glm::ivec2(P.x + 1, P.y + 1),
    };
    // clang-format on

    std::erase_if(loadedChunks, [&](const Chunk& chunk) {
        for (const auto& pos : chunkPositions) {
            if (chunk.chunkPosition == pos) {
                return false; // keep
            }
        }
        return true; // erase if no match
    });
}

void MapSystem::generateChunk(glm::ivec2 newPos) {
    Chunk newChunk{ newPos };
    newChunk.worldPosition = chunkSize * glm::vec2(newPos);
    loadedChunks.push_back(newChunk);
}

glm::ivec2 MapSystem::worldToChunk(const glm::vec2& pos) {
    int cx = static_cast<int>(floor(pos.x / chunkSize));
    int cy = static_cast<int>(floor(pos.y / chunkSize));
    return { cx, cy };
}
