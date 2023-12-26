/**
 * Collection of helper functions related to OpenGL data types.
 * These functions provide a baseline for creating a renderer API agnostic
 * framework.
 */

#pragma once

#include <glad/glad.h>

namespace Framework {
    enum class ShaderDataType {
        None = 0,
        Float, Float2, Float3, Float4,
        Mat3, Mat4,
        Int, Int2, Int3, Int4,
        Bool
    };

    /** Return how many bytes a type occupies in memory */
    constexpr GLsizei ShaderDataTypeSize(ShaderDataType type) {
        switch(type) {
            case ShaderDataType::Float:     return 4;
            case ShaderDataType::Float2:    return 4 * 2;
            case ShaderDataType::Float3:    return 4 * 3;
            case ShaderDataType::Float4:    return 4 * 4;
            case ShaderDataType::Mat3:      return 4 * 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4 * 4;
            case ShaderDataType::Int:       return 4;
            case ShaderDataType::Int2:      return 4 * 2;
            case ShaderDataType::Int3:      return 4 * 3;
            case ShaderDataType::Int4:      return 4 * 4;
            case ShaderDataType::Bool:      return 1;
            case ShaderDataType::None:      return 0;
        }

        return 0;
    }

    /** Return the OpenGL equivalent to a type */
    constexpr GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type) {
        switch(type) {
            case ShaderDataType::Float:     return GL_FLOAT;
            case ShaderDataType::Float2:    return GL_FLOAT;
            case ShaderDataType::Float3:    return GL_FLOAT;
            case ShaderDataType::Float4:    return GL_FLOAT;
            case ShaderDataType::Mat3:      return GL_FLOAT;
            case ShaderDataType::Mat4:      return GL_FLOAT;
            case ShaderDataType::Int:       return GL_INT;
            case ShaderDataType::Int2:      return GL_INT;
            case ShaderDataType::Int3:      return GL_INT;
            case ShaderDataType::Int4:      return GL_INT;
            case ShaderDataType::Bool:      return GL_INT;
            case ShaderDataType::None:      return GL_INT;
        }

        return GL_INT;
    }

    /** Return how many components a type has */
    constexpr GLsizei ShaderDataTypeComponentCount(ShaderDataType type) {
        switch(type) {
            case ShaderDataType::Float:     return 1;
            case ShaderDataType::Float2:    return 2;
            case ShaderDataType::Float3:    return 3;
            case ShaderDataType::Float4:    return 4;
            case ShaderDataType::Mat3:      return 3 * 3;
            case ShaderDataType::Mat4:      return 4 * 4;
            case ShaderDataType::Int:       return 1;
            case ShaderDataType::Int2:      return 2;
            case ShaderDataType::Int3:      return 3;
            case ShaderDataType::Int4:      return 4;
            case ShaderDataType::Bool:      return 1;
            case ShaderDataType::None:      return 0;
        }

        return 0;
    }
}
