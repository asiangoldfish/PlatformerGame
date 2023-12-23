#include "IndexBuffer.h"

namespace Framework {
    IndexBuffer::IndexBuffer(uint32_t *indices, int count) {
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, (uint32_t)(count * sizeof(int)), indices, GL_STATIC_DRAW);

        this->count = count;
    }
    
    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &indexBufferID);
    }
    
    void IndexBuffer::bind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    }
    
    void IndexBuffer::unbind() const {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::create(
        GLuint* indices, GLsizei count) {
        
        return std::make_shared<IndexBuffer>(indices, count);
    }
}
