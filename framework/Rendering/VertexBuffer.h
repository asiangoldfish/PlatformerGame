#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <glad/glad.h>
#include "BufferLayout.h"
#include <memory>

namespace Framework {
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
}

#endif // VERTEX_BUFFER_H