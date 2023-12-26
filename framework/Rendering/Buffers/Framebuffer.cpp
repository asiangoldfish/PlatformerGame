// External libraries
#include <glad/glad.h>

// Framework
#include "Framebuffer.h"
#include "TextureManager.h"

namespace FW {
    Framebuffer::Framebuffer()
    {
        createFramebuffer();
    }

    Framebuffer::~Framebuffer()
    {
        glDeleteFramebuffers(1, &framebuffer);
        glDeleteTextures(1, &colorAttachment);
        glDeleteTextures(1, &depthAttachment);
    }

    void Framebuffer::createFramebuffer()
    {
        glActiveTexture(GL_TEXTURE0);
        glCreateFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        // Color - RGBA
        glCreateTextures(GL_TEXTURE_2D, 1, &colorAttachment);
        glBindTexture(GL_TEXTURE_2D, colorAttachment);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RGBA8,
                     1280,
                     720,
                     0,
                     GL_RGBA,
                     GL_UNSIGNED_BYTE,
                     nullptr);
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
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, 800, 600);

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
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
    }

    void Framebuffer::unbind() const
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void Framebuffer::bindTexture() const
    {
        glBindTexture(GL_TEXTURE_2D, colorAttachment);
    }
} // Framework
