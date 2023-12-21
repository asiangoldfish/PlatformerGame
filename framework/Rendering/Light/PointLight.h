#ifndef PLATFORMERGAME_POINTLIGHT_H
#define PLATFORMERGAME_POINTLIGHT_H

#include "Light.h"

namespace Framework {

    class PointLight : public Light
    {
    public:
        PointLight() { numOfPointLights++; }
        virtual ~PointLight() { numOfPointLights--; }

        void draw() override;

        void setConstant(const float c) { constant = c; }
        void setLinear(const float l) { linear = l; }
        void setQuadratic(const float q) { quadratic = q; }

        const glm::vec3& getPosition() { return position; }
        void setPosition(const glm::vec3 pos) { position = pos; }

        float getBrightness() const { return brightness; }
        void setBrightness(float value) { brightness = value; }

    private:
        glm::vec3 position{ 0.0f };
        float brightness = 1.0f;

        /** Constant should normally be set to 0 */
        float constant = 1.0f;
        float linear = 1.0f;
        float quadratic = 1.0f;

        static int numOfPointLights;
    };

} // Rendering

#endif // PLATFORMERGAME_POINTLIGHT_H
