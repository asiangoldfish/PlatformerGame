#pragma once

#include <glm/glm.hpp>

#include "pch.h"

namespace Framework {
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
} // Framework
