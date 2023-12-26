#include <sstream>

#include "PointLight.h"

#include "Log.h"

namespace FW {
    int PointLight::numOfPointLights = 0;

    void PointLight::draw(const ref<Shader>& shader)
    {
        std::stringstream ss;
        ss << "u_pointLight[" << numOfPointLights - 1 << "]";

        shader->setFloat3(ss.str() + ".ambient", ambient);
        shader->setFloat3(ss.str() + ".diffuse", diffuse);
        shader->setFloat3(ss.str() + ".specular", specular);

        shader->setFloat3(ss.str() + ".position", position);

        shader->setFloat(ss.str() + ".constant", constant);
        shader->setFloat(ss.str() + ".linear", linear);
        shader->setFloat(ss.str() + ".quadratic", quadratic);

        shader->setInt("numOfPointLights", numOfPointLights);

        shader->setFloat(ss.str() + ".brightness", brightness);
    }
} // Framework
