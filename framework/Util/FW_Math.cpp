#include "FW_Math.h"
#include "doctest/doctest.h"

/**
 * Interpolate between `v0` and `v1` by `t`.
 *
 * To save performance, `t` isn't clamped between 0 and 1.
 */
float lerp(float v0, float v1, float t) {
    return (1 - t) * v0 + t * v1;
}

DOCTEST_TEST_CASE("Testing linear interpoloation: lerp(v0, v1, t)") {
    DOCTEST_CHECK(lerp(0.0f, 1.0f, 0.5f) == 0.5f);
    DOCTEST_CHECK(lerp(0.0f, 1.0f, 0.0f) == 0.0f);
    DOCTEST_CHECK(lerp(0.0f, 1.0f, -1.f) < 0.0f);  // Input less than v0
    DOCTEST_CHECK(lerp(0.0f, 1.0f, -0.1f) < 0.0f); // Input less than v0
    DOCTEST_CHECK(lerp(0.0f, 1.0f, 1.1f) > 1.0f);  // Input greater than v1
    DOCTEST_CHECK(lerp(0.0f, 1.0f, 10.0f) > 1.0f); // Input greater than v1
    DOCTEST_CHECK(lerp(0.0f, 1.0f, 1.0f) == 1.0f);
}

float lerp(float t) {
    return lerp(0.0f, 1.0f, t);
}

DOCTEST_TEST_CASE("Testing linear interpoloation: lerp(v0, v1, t)") {
    DOCTEST_CHECK(lerp(0.5f) == 0.5f);
    DOCTEST_CHECK(lerp(0.0f) == 0.0f);
    DOCTEST_CHECK(lerp(-1.f) < 0.0f);  // Input less than v0
    DOCTEST_CHECK(lerp(-0.1f) < 0.0f); // Input less than v0
    DOCTEST_CHECK(lerp(1.1f) > 1.0f);  // Input greater than v1
    DOCTEST_CHECK(lerp(10.0f) > 1.0f); // Input greater than v1
    DOCTEST_CHECK(lerp(1.0f) == 1.0f);
}

float sstep3(float x) {
    return x * x * (3 - 2 * x);
}

DOCTEST_TEST_CASE("Testing sstep3 function") {
    DOCTEST_SUBCASE("Boundary values") {
        DOCTEST_CHECK(sstep3(0.0f) == doctest::Approx(0.0f)); // At x = 0
        DOCTEST_CHECK(sstep3(1.0f) == doctest::Approx(1.0f)); // At x = 1
    }

    DOCTEST_SUBCASE("Intermediate values") {
        DOCTEST_CHECK(sstep3(0.5f) == doctest::Approx(0.5f)); // At x = 0.5
        DOCTEST_CHECK(sstep3(0.25f) ==
                      doctest::Approx(0.15625f)); // At x = 0.25
        DOCTEST_CHECK(sstep3(0.75f) ==
                      doctest::Approx(0.84375f)); // At x = 0.75
    }

    DOCTEST_SUBCASE("Out-of-bound values") {
        DOCTEST_CHECK(sstep3(-0.5f) == doctest::Approx(1.0f)); // At x = -0.5
        DOCTEST_CHECK(sstep3(1.5f) == doctest::Approx(0.0f));  // At x = 1.5
    }
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
