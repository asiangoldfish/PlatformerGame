#pragma once

// C++ libraries
#include <memory>

// External
#include "glm/glm.hpp"

// Framework
#include "Shader.h"

// Pre-declared classes
class Shader;

/**
 * Bring lighting to the scene
 */
class Light {
public:
    Light() = default;
    ~Light() = default;

    /** Draw the light caster. */
    virtual void draw(const ref<Shader>& shader) = 0;

    /**
     * Set whether this light should be active.
     *
     * If this light caster is disabled, then objects affected by it will no
     * longer cast light on objects.
     * @param e True to enable the light caster, or false to disable.
     */
    void setEnable(const bool e) { enable = e; }

    /**
     * Set the shader to upload light properties to.
     *
     * Currently, the Framework only supports a single shader.
     * @param s Address of the shader to upload properties to.
     */
    void setShader(ref<Shader> s) { this->shader = s; }

    void setAmbient(const glm::vec3& a) { ambient = a; }
    void setDiffuse(const glm::vec3& d) { diffuse = d; }
    void setSpecular(const glm::vec3& s) { specular = s; }

protected:
    ref<Shader> shader = nullptr;

    // Light attributes
    glm::vec3 ambient{ 1.0f };
    glm::vec3 diffuse{ 1.0f };
    glm::vec3 specular{ 1.0f };

private:
    bool enable = true;
};
