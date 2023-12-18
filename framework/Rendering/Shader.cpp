#include "Shader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cassert>
#include "assertions.h"
#include <fstream>
#include <string>

namespace Framework {

    Shader::Shader(
        const std::string &vertexSrc,
        const std::string &fragmentSrc,
        bool isFilepath
    ) {
        if(isFilepath) {
            vertexShader = compileShader(GL_VERTEX_SHADER, readFile(vertexSrc));
            fragmentShader = compileShader(GL_FRAGMENT_SHADER, readFile(fragmentSrc));
        } else {
            vertexShader = compileShader(GL_VERTEX_SHADER, vertexSrc);
            fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSrc);
        }

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        // Output shader program error log
        int success;
        char infoLog[512];

        glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
        if (success) {  // Successfully compiled and link shader program
            glDeleteShader(vertexShader);
            glDeleteShader(fragmentShader);
        } else {        // Failed to compile and link shader program
            glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }
    }

    Shader::~Shader() {
        // TODO: Release the shader program resources.
    }

    void Shader::bind() const {
        glUseProgram(shaderProgram);
    }

    void Shader::unbind() const {
        glUseProgram(0);
    }

    GLuint Shader::compileShader(
        GLenum shaderType,
        const std::string &shaderSrc
    ) {
        GLuint shader = glCreateShader(shaderType);
        const GLchar* shaderCSrc = shaderSrc.c_str();
        glShaderSource(shader, 1, &shaderCSrc, nullptr);
        glCompileShader(shader);

        // Throw exception on error with log
        int success;
        char infoLog[512];
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 512, NULL, infoLog);

            std::string typeName;

            switch(shaderType) {
                case GL_VERTEX_SHADER:
                    typeName = "VERTEX";
                    break;
                case GL_FRAGMENT_SHADER:
                    typeName = "FRAGMENT";
                    break;
                default:
                    break;

            }

            std::cout
                << "ERROR::SHADER::"
                << typeName
                << "::COMPILATION_FAILED\n"
                << infoLog
                << std::endl;

                framework_assert();
        }

        return shader;
    }

    std::string Shader::readFile(const std::string &filepath) {
        std::string result;
        std::ifstream infile(filepath, std::ios::in | std::ios::binary);

        if (infile) {
            infile.seekg(0, std::ios::end);
            result.resize(infile.tellg());
            infile.seekg(0, std::ios::beg);
            infile.read(&result[0], result.size());
            infile.close();
        } else {
            framework_assert("Unable to read file " + filepath);
        }

        return result;
    }

    void Shader::setInt(const std::string& name, const int value) {
        int location = glGetUniformLocation(shaderProgram, name.c_str());
        if (location < 0) {
            framework_warn("Unable to upload uniform " + name);
        }
        glUniform1i(location, value);
    }

    void Shader::setFloat(const std::string& name,const float value) {
        int location = glGetUniformLocation(shaderProgram,name.c_str());
        if (location < 0) {
            framework_warn("Unable to upload uniform " + name);
        }
        glUniform1f(location, value);
    }


    void Shader::setFloat2(const std::string &name,
                                     const glm::vec2 &vector) {
        int location = glGetUniformLocation(shaderProgram,name.c_str());
        if (location < 0) {
            framework_warn("Unable to upload uniform " + name);
        }
        glUniform2f(location, vector.x, vector.y);
    }

    void Shader::setFloat3(const std::string &name,
                                     const glm::vec3 &vector) {
        int location = glGetUniformLocation(shaderProgram,name.c_str());
        if (location < 0) {
            framework_warn("Unable to upload uniform " + name);
        }
        
        glUniform3f(location, vector.x, vector.y, vector.z);
    }

    void Shader::setFloat4(const std::string& name,
        const glm::vec4& vector) {
        int location = glGetUniformLocation(shaderProgram, name.c_str());
        if (location < 0) {
            framework_warn("Unable to upload uniform " + name);
        }

        glUniform4f(location, vector.x, vector.y, vector.z, vector.w);
    }

    void Shader::setMat4(const std::string &name,
                                   const glm::mat4 &matrix) {
        int location = glGetUniformLocation(shaderProgram, name.c_str());
        if (location < 0) {
            framework_warn("Unable to upload uniform " + name);
        }
        
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
    }
} // namespace Framework
