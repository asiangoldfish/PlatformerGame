#pragma once

#include "pch.h"
#include <random>

namespace FW {
    float lerp(float v0, float v1, float t);

    float lerp(float t);

    float sstep3(float x);

    constexpr double PI = 3.1415926535897932384626433;

    /**
     * @brief Generate a random number between 0.0 and 1.0
     * @return Random number between 0.0 and 1.0
     */
    float rng();

    /**
     * @brief Generate a random number between named low and high value
     * @param low Lowest number to generate
     * @param high Highest number to generate
     * @return Random number between lowest and highest number
     */
    float rng(float low, float high);
}
