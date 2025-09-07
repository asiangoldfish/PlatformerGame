/**
 * An extension to the GLM library.
 */

#pragma once

#include <glm/glm.hpp>
#include <functional>

// Hashing - Extend std::hash to hash custom types.
// Docs: https://en.cppreference.com/w/cpp/utility/hash.html
namespace std {
    template<>
    struct hash<glm::ivec2> {
        size_t operator()(const glm::ivec2& k) const {
            return std::hash<int>()(k.x) ^ std::hash<int>()(k.y);
        }

        bool operator()(const glm::ivec2& a, const glm::ivec2& b) const {
            return a.x == b.x && a.y == b.y;
        }
    };
}