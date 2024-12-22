#pragma once

#include "pch.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

namespace FW {
    class Viewport {
    public:
        void setSize(glm::vec2 size) {
            this->size = size;
            glViewport(0, 0, size.x, size.y);
        }

    private:
        glm::vec2 size;
    };
} // namespace FW