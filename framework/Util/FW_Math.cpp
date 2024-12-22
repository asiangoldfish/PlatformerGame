#include "FW_Math.h"

namespace FW {
    float lerp(float v0, float v1, float t) {
        return (1 - t) * v0 + t * v1;
    }

    float lerp(float t) {
        return lerp(0.0f, 1.0f, t);
    }

    float sstep3(float x) {
        return x * x * (3 - 2 * x);
    }

    float rng() {
        // Setup random number generator
        std::random_device rnddev;
        std::mt19937 gen(rnddev());
        std::uniform_real_distribution<float> dist(0.f, 1.f);

        float myRandomNumber = dist(gen);

        return myRandomNumber;
    }

    float rng(float low, float high) {
        return rng() * (high - low) + low;
    }
}
