#pragma once

#include "Light.h"

namespace Framework {

    class SpotLight : public Light
    {
        void draw() override;

    private:
        glm::vec3 position{ 0.0f };
    };

} // Rendering
