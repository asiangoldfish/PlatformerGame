#include "ShaderManager.h"
#include "Log.h"

namespace FW {
    void ShaderManager::createShaderFromFiles(const std::string& name,
                                              const std::string& vertexSrc,
                                              const std::string& fragmentSrc) {

        auto iter = shaders.find(name);
        if (iter == shaders.end()) {
            shaders.try_emplace(name, Shader{ vertexSrc, fragmentSrc });
        }
    }

    Shader* ShaderManager::bind(const std::string& name) {
        if (Shader* shader = getShader(name)) {
            shader->bind();
            return shader;
        }

        return nullptr;
    }

    Shader* ShaderManager::getShader(const std::string& name) {
        auto iter = shaders.find(name);
        if (iter != shaders.end()) {
            return &iter->second;
        } else {
            return nullptr;
        }
    }

    void ShaderManager::clear() {
        shaders.clear();
    }
}
