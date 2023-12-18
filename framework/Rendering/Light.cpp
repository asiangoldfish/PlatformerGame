#include "Light.h"

#include <utility>

#include "Shader.h"

namespace Framework {
	Light::Light(
        Shader* shader,
        const glm::vec3& position
        )
		: shader(shader)
        , position(position)
        {}

	// Uploads to the hardcoded uniforms in the frag shader.
	// Therefore, we only support a single light source.
	void Light::update() {
        if (shader) {
            shader->setFloat3("u_lightSourcePosition", position);
            shader->setFloat3("u_diffuseColor", diffuseColor);
            shader->setFloat3("u_specularColor", specularColor);
            shader->setInt("u_enableLighting", enableLighting);
        }
	}
}
