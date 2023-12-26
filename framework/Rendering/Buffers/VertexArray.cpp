#include "VertexArray.h"
#include "glad/glad.h"

#include <iostream>

namespace FW {
    VertexArray::VertexArray() {
        // Create new VAO
        glGenVertexArrays(1, &vertexArrayID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteBuffers(1, &vertexArrayID);
    }

    void VertexArray::bind() const {
        glBindVertexArray(vertexArrayID);
    }

    void VertexArray::unbind() const {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(
            VertexBuffer* vertexBuffer) {

        glBindVertexArray(vertexArrayID);
        vertexBuffer->bind();

        // Set vertex attributes
        int index = 0;
        const auto& layout = vertexBuffer->getLayout();
        for (const auto& attribute : layout) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index,
                (GLint)attribute.getComponentCount(),
                ShaderDataTypeToOpenGLBaseType(attribute.type), 
                attribute.normalized ? GL_TRUE : GL_FALSE, 
                layout.getStride(),
                (const void*)attribute.offset
            );
            index++;
        }

        vertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::setIndexBuffer(
        IndexBuffer* indexBuffer) {
            glBindVertexArray(vertexArrayID);
            indexBuffer->bind();
            this->indexBuffer = indexBuffer;
    }

    std::shared_ptr<VertexArray> VertexArray::create()
    {
        return std::make_shared<VertexArray>();
    }

}
