#pragma once

#include "Light.h"

namespace FW {

    class SpotLight : public Light
    {
        void draw(const ref<Shader>& shader) override;

    private:
        glm::vec3 position{ 0.0f };
    };

} // Rendering
