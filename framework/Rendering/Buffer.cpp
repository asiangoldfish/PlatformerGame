#include "Buffer.h"

namespace FW {
#pragma region Vertex Array
    VertexArray::VertexArray()
    {
        // Create new VAO
        glGenVertexArrays(1, &vertexArrayID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteBuffers(1, &vertexArrayID);
    }

    void VertexArray::bind() const
    {
        glBindVertexArray(vertexArrayID);
    }

    void VertexArray::unbind() const
    {
        glBindVertexArray(0);
    }

    void VertexArray::addVertexBuffer(VertexBuffer* vertexBuffer)
    {

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
              (const void*)attribute.offset);
            index++;
        }

        vertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::setIndexBuffer(IndexBuffer* indexBuffer)
    {
        glBindVertexArray(vertexArrayID);
        indexBuffer->bind();
        this->indexBuffer = indexBuffer;
    }

    std::shared_ptr<VertexArray> VertexArray::create()
    {
        return std::make_shared<VertexArray>();
    }
#pragma endregion

#pragma region Vertex Buffer
    VertexBuffer::VertexBuffer(const void* vertices,
                               GLsizei size,
                               GLenum drawMethod)
    {
        glGenBuffers(1, &vertexBufferId);
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, drawMethod);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &vertexBufferId);
    }

    void VertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
    }

    void VertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    void VertexBuffer::bufferSubData(GLintptr offset,
                                     GLsizeiptr size,
                                     const void* data) const
    {

        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }

    std::shared_ptr<VertexBuffer> VertexBuffer::create(const void* vertices,
                                                       GLsizei size,
                                                       GLenum drawMethod)
    {

        return std::make_shared<VertexBuffer>(vertices, size, drawMethod);
    }
#pragma endregion

#pragma region Index Buffer
    IndexBuffer::IndexBuffer(uint32_t* indices, int count)
    {
        glGenBuffers(1, &indexBufferID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     (uint32_t)(count * sizeof(int)),
                     indices,
                     GL_STATIC_DRAW);

        this->count = count;
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &indexBufferID);
    }

    void IndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferID);
    }

    void IndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    std::shared_ptr<IndexBuffer> IndexBuffer::create(GLuint* indices,
                                                     GLsizei count)
    {

        return std::make_shared<IndexBuffer>(indices, count);
    }
#pragma endregion

#pragma region Framebuffer
    Framebuffer::Framebuffer()
    {
        createFramebuffer();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &colorAttachment);
        glDeleteTextures(1, &depthAttachment);
    }

    void Framebuffer::resize(const glm::vec2& new_size) {
        glDeleteFramebuffers(1, &fbo);
        glDeleteTextures(1, &colorAttachment);
        glDeleteTextures(1, &depthAttachment);

        createFramebuffer(new_size);
    }

    void Framebuffer::createFramebuffer(glm::vec2 size) {
        this->size = size;

        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        glGenTextures(1, &colorAttachment);
        glBindTexture(GL_TEXTURE_2D, colorAttachment);

        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGB,
                     size.x,
                     size.y,
                     0,
                     GL_RGB,
                     GL_UNSIGNED_BYTE,
                     NULL);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_COLOR_ATTACHMENT0,
                               GL_TEXTURE_2D,
                               colorAttachment,
                               0);

        // Depth
        glCreateTextures(GL_TEXTURE_2D, 1, &depthAttachment);
        glBindTexture(GL_TEXTURE_2D, depthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, size.x, size.y);

        glFramebufferTexture2D(GL_FRAMEBUFFER,
                               GL_DEPTH_STENCIL_ATTACHMENT,
                               GL_TEXTURE_2D,
                               depthAttachment,
                               0);

        GLenum framebufferStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

        if (framebufferStatus != GL_FRAMEBUFFER_COMPLETE) {
            // Framebuffer is incomplete, print error details
            switch (framebufferStatus) {
                case GL_FRAMEBUFFER_UNDEFINED:
                    WARN("Framebuffer is incomplete: GL_FRAMEBUFFER_UNDEFINED");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                    WARN("Framebuffer is incomplete: "
                         "GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                    WARN("Framebuffer is incomplete: "
                         "GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                    WARN("Framebuffer is incomplete: "
                         "GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                    WARN("Framebuffer is incomplete: "
                         "GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER");
                    break;
                case GL_FRAMEBUFFER_UNSUPPORTED:
                    WARN(
                      "Framebuffer is incomplete: GL_FRAMEBUFFER_UNSUPPORTED");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
                    WARN("Framebuffer is incomplete: "
                         "GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE");
                    break;
                case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
                    WARN("Framebuffer is incomplete: "
                         "GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS");
                    break;
                default:
                    WARN("Framebuffer is incomplete with unknown error: {}",
                         framebufferStatus);
                    break;
            }
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    }

    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_2D, colorAttachment);
    }
#pragma endregion
}
