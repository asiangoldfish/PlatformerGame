#include "VertexBuffer.h"

#include <GLFW/glfw3.h>

namespace Framework {
    VertexBuffer::VertexBuffer(const void *vertices, GLsizei size, GLenum drawMethod) {
        glGenBuffers(1, &vertexBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, drawMethod);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &vertexBufferId);
    }

    void VertexBuffer::bind() const {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    }

    void VertexBuffer::unbind() const {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::bufferSubData(GLintptr offset, GLsizeiptr size,
                                     const void *data) const {

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::create(
        const void* vertices,
        GLsizei size,
        GLenum drawMethod
    ) {

        return std::make_shared<VertexBuffer>(vertices, size, drawMethod);
    }
}