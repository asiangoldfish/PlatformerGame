#ifndef PLATFORMERGAME_SPOTLIGHT_H
#define PLATFORMERGAME_SPOTLIGHT_H

#include "Light.h"

namespace Framework {

    class SpotLight : public Light
    {
        void draw() override;

    private:
        glm::vec3 position{ 0.0f };
    };

} // Rendering

#endif // PLATFORMERGAME_SPOTLIGHT_H
