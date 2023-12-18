#ifndef __MATH__HPP__
#define __MATH__HPP__

#include <algorithm>
#include <random>
#include <iostream>

float lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

float lerp(float t) {
    return lerp(0.0f, 1.0f, t);
}

float sstep3(float x) {
    return x * x * (3 - 2 * x);
}

constexpr double PI = 3.1415926535897932384626433;

/**
 * @brief Generate a random number between 0.0 and 1.0
 * @return Random number between 0.0 and 1.0
*/
float rng()
{
    // Setup random number generator
    std::random_device rnddev;
    std::mt19937 gen(rnddev());
    std::uniform_real_distribution<float> dist(0.f, 1.f);

    float myRandomNumber = dist(gen);

    return myRandomNumber;
}

/**
 * @brief Generate a random number between named low and high value
 * @param low Lowest number to generate
 * @param high Highest number to generate
 * @return Random number between lowest and highest number
*/
float rng(float low, float high)
{
    return rng() * (high - low) + low;
}

#endif // __MATH__HPP__