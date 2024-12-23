#pragma once

#include "Light.h"

class SpotLight : public Light {
public:
    void draw(const ref<Shader>& shader) override;

private:
    glm::vec3 position{ 0.0f };
};
