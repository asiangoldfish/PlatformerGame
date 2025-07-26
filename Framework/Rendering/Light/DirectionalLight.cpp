#include <sstream>

#include "DirectionalLight.h"

namespace FW {
    int DirectionalLight::numOfDirectionalLights = 0;

    void DirectionalLight::draw(const ref<Shader>& shader)
    {
        std::stringstream ss;
        ss << "u_directionalLight[" << numOfDirectionalLights - 1 << "]";
        shader->setFloat3(ss.str() + ".ambient", ambient);
        shader->setFloat3(ss.str() + ".diffuse", diffuse);
        shader->setFloat3(ss.str() + ".specular", specular);
        shader->setFloat3(ss.str() + ".direction", direction);
        shader->setInt("numOfDirectionalLights", numOfDirectionalLights);
    }
} // FW
