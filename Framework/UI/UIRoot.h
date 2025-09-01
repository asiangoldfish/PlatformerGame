#pragma once

#include "Entity.h"
#include "Camera/Camera.h"

namespace FW::UI {
    /**
     * This class is a barebone class for UI with minimal functionality.
     *
     * It comes in handy when composing a more complex element consisting of
     * several other elements, such as a ProgressBar that consists of a
     * foreground and a background element. It can also be used to group nodes.
     *
     * The UIRoot comes with the transformation component by default.
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

    protected:
        /** While the UIRoot does not utilise the camera, subclasses may. */
        ref<Camera> camera;
    };
}