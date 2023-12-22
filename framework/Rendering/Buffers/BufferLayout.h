#ifndef BUFFERLAYOUT_H
#define BUFFERLAYOUT_H

#include "BufferLayout.h"
#include "ShaderDataTypes.h"
#include "GLFW/glfw3.h"

#include <string>
#include <vector>

namespace Framework {

    /** An attribute for a Vertex Buffer Object. */
    struct BufferAttribute {
        BufferAttribute(
            ShaderDataType type,
            const std::string& name,
            GLboolean normalized = false
        ) 
            : name(name)
            , type(type)
            , size(ShaderDataTypeSize(type))
            , offset(0)
            , normalized(normalized)
        {}

        /** Get the number of components that a type has */
        uint32_t getComponentCount() const
        {
            switch (type) {
                case ShaderDataType::Float:
                    return 1;
                case ShaderDataType::Float2:
                    return 2;
                case ShaderDataType::Float3:
                    return 3;
                case ShaderDataType::Float4:
                    return 4;
                case ShaderDataType::Mat3:
                    return 3 * 3;
                case ShaderDataType::Mat4:
                    return 4 * 4;
                case ShaderDataType::Int:
                    return 4;
                case ShaderDataType::Int2:
                    return 2;
                case ShaderDataType::Int3:
                    return 3;
                case ShaderDataType::Int4:
                    return 4;
                case ShaderDataType::Bool:
                    return 1;
            }
            return 0;
        }

        /**
         * The components name. Does not impact anything besides visually
         * helping developers to identify what attribute this is.
         */
        std::string name;

        /** Attribute type. Must match with the type in the GLSL shader. */
        ShaderDataType type;

        /** Size occupied by the attribute in bytes. */
        uint32_t size;

        /** At what byte does this attribute start at. */
        uint32_t offset;

        /** Should the attribute be normalized? */
        GLboolean normalized;
    };

    /**
     * Layout for Vertex Buffer Object.
     *
     * A Vertex Buffer Object has attributes in a buffer. The GPU doesn't know
     * how to handle them, so by using a Buffer Layout we can tell the GPU
     * what byte is what attribute.
     */
    class BufferLayout {
    public:
        BufferLayout() = default;
        BufferLayout(const std::initializer_list<BufferAttribute>& attributes)
            : attributes(attributes)
        {
                this->calculateOffsetAndStride();
        }

        /** Get all attributes */
        inline const std::vector<BufferAttribute>& getAttributes() const {
            return attributes;
        }

        inline GLsizei getStride() const { return stride; }

        std::vector<BufferAttribute>::iterator begin() {
            return attributes.begin();
        }
        std::vector<BufferAttribute>::iterator end() {
            return attributes.end();
        }
        std::vector<BufferAttribute>::const_iterator begin() const {
            return attributes.begin();
        }
        std::vector<BufferAttribute>::const_iterator end() const {
            return attributes.end();
        }

    private:
        /**
         * Automatically compute attributes offset and stride
         */
        void calculateOffsetAndStride() {
            GLuint offset = 0;
            stride = 0;
            for (auto& attribute: attributes) {
                attribute.offset = offset;
                offset += attribute.size;
                stride += attribute.size;
            }
        }

    private:
        /**
         * There may be several attributes. Example: Position, Color...
         * Each element in the vector represents one unique attribute.
         */
        std::vector<BufferAttribute> attributes;

        /**
         * The Stride is the distance in bytes between an attribute in one
         * vertex to the next vertex. Example: Suppose we have the following
         * attributes:
         *
         * // Pos   // Color
         *  0, 1,     1, 1, 0, 1,
         *  1, 0,     1, 1, 0, 1
         *
         *  The Stride would be from Pos in the first vertex (first line) for
         *  the x-value (that is 1) to where the next x-value is. In this
         *  example the Stride is 6 bytes(6 elements * sizeof(float) = 24 bytes)
         */
        GLuint stride = 0;
    };
}

#endif // BUFFERLAYOUT_H
