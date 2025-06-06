#pragma once

// External libraries
#include <glm/glm.hpp>

// Framework
#include "Light.h"

/**
 * Cast light on objects regardless of angle and distance.
 *
 * Directional lights are meant for light casters that are far away from the
 * scene. A perfect example is the sun. Only the angle between the object
 * and the caster will impact the lighting effect.
 */
class DirectionalLight : public Light {
public:
    DirectionalLight() { numOfDirectionalLights++; }
    virtual ~DirectionalLight() { numOfDirectionalLights--; }

    void draw(const ref<Shader>& shader) override;

    void setDirection(const glm::vec3& d) { direction = d; }

private:
    glm::vec3 direction{ 0.0f };

    /** Globally keep track of how many directional lights exist */
    static int numOfDirectionalLights;
};
