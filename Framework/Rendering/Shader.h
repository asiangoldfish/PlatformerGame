#pragma once

#include "pch.h"
#include <string>
#include <glm/glm.hpp>
#include <glad/glad.h>

#include "RenderCommands.h"

//#define SHADER_LOCATION_OUTPUT
#ifdef SHADER_LOCATION_OUTPUT
#define OUTPUT_SHADER_LOCATION_LOG(location, name) outputShaderLocationLog(location, name)
#else
#define OUTPUT_SHADER_LOCATION_LOG(location, name)
#endif

namespace FW {
    /**
     * Shader class for OpenGL.
     *
     * The shader is responsible for determining the rules for what is rendered
     * on the screen.
     *
     * The Shader does not support hot swapping shader source code, so they must
     * be passed on initialization. Upon initialization, they will be compiled
     * and the source code are discarded. Only the Shader Program remains and
     * will be bound.
     *
     * Shader is a non-copyable class. Credits to non-copyable implementation:
     * https://www.codeproject.com/Tips/1220451/Cplusplus-Tip-Make-Your-Class-Non-copyable-Without
     */
    class Shader {
    public:
        /**
         * @brief Create a new shader
         *
         * @param vertexSrc Source to vertex shader
         * @param fragmentSrc Source to fragment shader
         * @param isFilepath If true: source are file paths. If false: source are literal strings.
         * 
         * @example vertexSrc = R"(#version core 330 int main(){})"
        */
        Shader(
            const std::string& vertexSrc, 
            const std::string &fragmentSrc
        );
        virtual ~Shader();
        Shader() = default;

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        /** Copy constructor */
        Shader(Shader&& other) noexcept {
            shaderProgram = other.shaderProgram;
            other.shaderProgram = 0;
        }

        /** Move assignment operator */
        Shader& operator=(Shader&& other) noexcept {
            if (this != &other) {
                // free current resource before overwriting
                if (shaderProgram != 0) {
                    glDeleteProgram(shaderProgram);
                }
                shaderProgram = other.shaderProgram; // take ownership
                other.shaderProgram = 0;             // leave other safe
            }
            return *this;
        }

        /**
         * Bind the <u<Shader</u>.
         * @details Whenever the <u>Shader</u> is bound must be bound whenever
         * it is used, like uploading data to the GPU.
         */
        void bind() const;
        /** Unbind the shader. */
        void unbind() const;

        // Uniforms
        void setInt(const std::string& name, const int value) const;
        void setFloat(const std::string& name,const float value) const;
        void setFloat2(
            const std::string& name,
            const glm::vec2& vector
        ) const;
        void setFloat3(
            const std::string& name,
            const glm::vec3& vector
        ) const;
        void setFloat4(
            const std::string& name,
            const glm::vec4& vector
        ) const;

        void setMat4(
            const std::string& name,
            const glm::mat4& matrix
        ) const;

        /**
         * Shorthand to create a shared pointer to a Shader object.
         * @param vertexSrc Vertex source code.
         * @param fragmentSrc Fragment source code.
         * @param isFilepath If true, then the source code are fetched from
         * files on disk. If not, then the source code are expected to be string
         * literals.
         * @return Shared pointer to a Shader object.
         * @example
         * auto shader = Framework::Shader::create(vertexSrc, fragSrc);
         */
        static ref<Shader> create(const std::string& vertexSrc,
            const std::string &fragmentSrc) {
            return createRef<Shader>(vertexSrc, fragmentSrc);
        }

        /**
         * Upload the uniform set by RenderCommands::VisualizeMode.
         */
        void setVisualizeMode(RenderCommand::VisualizeMode mode) const;

    private:
        /** Compile a shader */
        GLuint compileShader(GLenum shaderType, const std::string& shaderSrc);

        /** Read shader source code from file on disk */
        std::string readFile(const std::string& filepath);

    private:
        /** Vertex shader ID */
        GLuint vertexShader = 0;

        /** Fragment shader ID */
        GLuint fragmentShader = 0;

    public:
        /** Shader program ID */
        GLuint shaderProgram = 0;
    };
}
