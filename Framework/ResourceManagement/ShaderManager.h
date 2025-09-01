#pragma once

#include <Shader.h>

#include <unordered_map>

namespace FW {
    /**
     * Credits to Scott Meyer for the Singleton pattern implementation.
     * https://laristra.github.io/flecsi/src/developer-guide/patterns/meyers_singleton.html
     */
    class ShaderManager {
    public:
        static ShaderManager& get() {
            static ShaderManager s;
            return s;
        }

        ShaderManager(const ShaderManager&) = delete;
        ShaderManager& operator=(const ShaderManager&) = delete;

        /** Explicitly clear the shader buffer. Should be called before the
         * application quits. */
        void clear();

    private:
        ShaderManager() {}
        ~ShaderManager() {}

    public:
        void createShaderFromFiles(const std::string& name,
                                   const std::string& vertexSrc,
                                   const std::string& fragmentSrc);

        /** If the shader cannot be found, then return false. */
        Shader* bind(const std::string& name);

    private:
        /** TODO find some way to avoid heap allocation */
        std::unordered_map<std::string, Shader> shaders;
    };
}
