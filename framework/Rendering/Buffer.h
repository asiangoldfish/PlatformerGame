#pragma once

#include "pch.h"

// External libraries
#include <glad/glad.h>
#include <glm/glm.hpp>

// Framework
#include "ShaderDataTypes.h"

namespace FW {
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;


#pragma region Buffer Layout
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
                case ShaderDataType::None:
                    break;
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
        int64_t offset;

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
#pragma endregion

#pragma region Vertex Array
    /**
     * Vertex Array Object in OpenGL.
     *
     * The Vertex Array class represents the Vertex Array Object in OpenGL.
     * It is a container for Vertex Buffer Objects and Element Buffer Objects.
     *
     * Note that this class supports several Vertex Buffer Objects, but only one
     * Element Buffer Object.
     */
    class VertexArray {
    public:
        /** Construct a Vertex Array Object. */
        VertexArray();
        virtual ~VertexArray();

        /** Bind the Vertex Array Object */
        void bind() const;

        /** Unbind the Vertex Array Object */
        void unbind() const;

        /**
         * Add a new Vertex Buffer Object
         * @param vertexBuffer Shared pointer to the Vertex Buffer Object
         */
        void addVertexBuffer(VertexBuffer* vertexBuffer);

        /**
         * Set the Element Buffer Object-
         * @param indexBuffer Element Buffer Object to set for this Vertex
         * Array Object.
         */
        void setIndexBuffer(IndexBuffer* indexBuffer);

        /**
         * Get a reference to the Element Buffer Object.
         * @return Shared pointer to the Element Buffer Object
         */
        const IndexBuffer* getIndexBuffer() const {
            return indexBuffer;
        }

        /**
         * Shortcut to create a new shared pointer to Vertex Array Object.
         * @return
         */
        static std::shared_ptr<VertexArray> create();

    private:
        /** This VAO's ID. Used to reference it in GLAD functions */
        uint32_t vertexArrayID = 0;

        /** Container with VBOs associated with this VAO */
        std::vector<VertexBuffer*> vertexBuffers;

        /** This VAO's associated EBO. */
        IndexBuffer* indexBuffer = nullptr;
    };
#pragma endregion

#pragma region Vertex Buffer
    /**
     * Vertex Buffer Object in OpenGL.
     *
     * The Vertex Buffer Object contains an object's vertex attributes.
     */
    class VertexBuffer {
    public:
        // Data buffer is bound upon construction
        VertexBuffer(
          const void *vertices,
          GLsizei size,
          GLenum drawMethod = GL_STATIC_DRAW
        );
        virtual ~VertexBuffer();

        void bind() const;
        void unbind() const;

        // Fill a specific segment of the buffer specified by an offset and size
        // with data
        void bufferSubData(
          GLintptr offset, GLsizeiptr size, const void *data) const;

        const BufferLayout& getLayout() const { return layout; }
        void setLayout(const BufferLayout& layout) { this->layout = layout; }

        static std::shared_ptr<VertexBuffer> create(
          const void* vertices,
          GLsizei size,
          GLenum drawMethod = GL_STATIC_DRAW);

    private:
        GLuint vertexBufferId = 0;
        BufferLayout layout;
    };
#pragma endregion

#pragma region Index Buffer
    /**
     * The IndexBuffer class represents the Element Buffer Object in OpenGL.
     */
    class IndexBuffer {
    public:
        /**
         * Construct the IndexBuffer class.
         *
         * During construction, create a new OpenGL Element Buffer Object.
         * @param indices Indices to upload to the GPU.
         * @param count How many indices to upload.
         *
         * @example
         * std::vector indices; // Generate the indices here
         * IndexBuffer* indexBuffer =
         *      new IndexBuffer(indices.data(), indices.size());
         */
        IndexBuffer(GLuint *indices, GLsizei count);
        virtual ~IndexBuffer();

        /** Bind this index buffer */
        void bind() const;
        /** Unbind this index buffer */
        void unbind() const;

        /** Get the count of how many indices there are in this buffer */
        inline GLuint getCount() const {
            return count;
        }

        /**
         * Shorthand for creating a new IndexBuffer shared pointer.
         *
         * @example
         * // Put the correct arguments for ::create()
         * auto indexBuffer = IndexBuffer::create();
         */
        static std::shared_ptr <IndexBuffer> create(
          GLuint* indices, GLsizei count);

    private:
        /** This index buffer's ID */
        GLuint indexBufferID = 0;
        /** How many indices there are in the buffer */
        GLuint count;
    };
#pragma endregion

#pragma region Framebuffer
    class Framebuffer
    {
    public:
        Framebuffer();
        virtual ~Framebuffer();

        void bind() const;
        void unbind() const;

        void bindTexture() const;
        [[nodiscard]] uint32_t getColorAttachment() const { return colorAttachment; }

    private:
        void createFramebuffer();

    private:
        /** The framebuffer's size */
        glm::vec2 size{ 0.0f };

        /** Identifier for the framebuffer object */
        uint32_t framebuffer;

        /** Identifier for the render buffer object */
        uint32_t renderBufferObject;
        uint32_t colorAttachment;
        uint32_t depthAttachment;
    };
#pragma endregion
}
