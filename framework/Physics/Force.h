#pragma once

#include "glm/glm.hpp"

class Force {
public:
    Force() = default;
    virtual ~Force() = default;
    virtual void update() = 0;
};

class GravityForce : public Force {
public:
    GravityForce() = default;
    virtual ~GravityForce() = default;

    virtual void update();

private:
    glm::vec3 acceleration{ 0.0f, -9.8067f, 0.0f };
};
