#ifndef LIGHT_H
#define LIGHT_H

// Libraries
#include <glm/glm.hpp>

// Built-in
#include <memory>

namespace Framework {
    // Pre-declared classes
    class Shader;

    /**
     * Bring lighting to the scene.
     *
     * Currently, the only supported shader model is the Phong Shader.
     * Therefore, the light only supports diffuse and specular colors.
     */
	class Light {
    public:
        Light() = default;
        Light(
            Shader* shader,
            const glm::vec3& position);
        ~Light() = default;

        void setPosition(const glm::vec3& newPosition)
        {
            position = newPosition;
        }

        void setDiffuseColor(const glm::vec3 diff)
        {
            diffuseColor = diff;
            update();
        }
        void setSpecularColor(const glm::vec3 spec)
        {
            specularColor = spec;
            update();
        }

        /** Uploads variables to frag shader. */
        void update();

        void setEnableLighting(const bool enable) { enableLighting = enable; }
        void setShader(Shader* shader) { this->shader = shader; }


    private:
        Shader* shader = nullptr;
        glm::vec3 position{0.0f};

        glm::vec3 diffuseColor{1.0f};
        glm::vec3 specularColor{1.0f};

        bool enableLighting = true;
    };
}


#endif // LIGHT_H
