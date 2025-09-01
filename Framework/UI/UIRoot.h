#pragma once

#include "Entity.h"
#include "Camera/Camera.h"

namespace FW::UI {
    /**
     * This class is a barebone class for UI with minimal functionality.
     *
     * By default, it is composed by the transformation component only, and its
     * primary purpose is to group child nodes.
     */
    class UIRoot : public Entity {
    public:
        UIRoot() = default;
        virtual ~UIRoot() = default;

        void init();
        ref<TransformationComponent> getTransform() {
            return transformationComponent;
        }

        glm::vec2 getPosition(glm::vec2 position) { return position; }
        void setPosition(const float x, const float y) {
            setPosition(glm::vec2{ x, y });
        }
        void setPosition(const glm::vec2& position);

        glm::vec2 getSize(glm::vec2 size) { return size; }
        void setSize(const glm::vec2& size) { this->size = size; }
        void setSize(const float x, const float y) { this->size = size; }

    private: // Basic UI stats
        glm::vec2 position{ 0.0f };
        glm::vec2 size{ 1.0f };

    private:
        ref<TransformationComponent> transformationComponent;
        std::string shader;
    };
}