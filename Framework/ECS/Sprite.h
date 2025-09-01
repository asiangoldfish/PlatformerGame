#pragma once

#include "Entity.h"
#include "Camera/Camera.h"

namespace FW {
    class Sprite : public Entity {
    public:
        Sprite();
        Sprite(ref<Camera> camera);
        virtual ~Sprite() = default;
        void init();
        std::string& getShader() { return shader; }

        glm::vec2 getSize();
        void setSize(float x, float y);
        void setSize(float uniformSize);
        void setSize(glm::vec2 size);
        
        glm::vec2 getPosition();
        void setPosition(float x, float y);
        void setPosition(glm::vec2 position);
        void moveBy(float x, float y);

        void setRotation(float yaw, float pitch, float roll);
        void setRotation(glm::vec3 rot);

        void update(float delta) override;
        void setCamera(ref<Camera> camera) { this->camera = camera; }

        // Colors
        glm::vec4 getColor() { return color; }
        void setColor(const glm::vec4& color) {
            this->color = color;
            drawableSetColor(this->color);
        }
        void setColor(const glm::vec3& color) {
            this->color = glm::vec4{ color, 1.0f };
            drawableSetColor(this->color);
        }
        void setColor(float r, float g, float b, float a) {
            color = glm::vec4{ r, g, b, a };
            drawableSetColor(this->color);
        }
        void setColor(float r, float g, float b) {
            color = glm::vec4{ r, g, b, 1.0f };
            drawableSetColor(this->color);
        }
        void setColor(float col) {
            color = glm::vec4{ col, col, col, 1.0f };
            drawableSetColor(this->color);
        }

    private:
        void drawableSetColor(const glm::vec4& col) {
            drawableComponent->color = col;
        }

    protected:
        ref<TransformationComponent> transformationComponent;
        ref<DrawableComponent> drawableComponent;
        std::string shader;

        /**
         * If the camera exists, then Sprite::draw() will draw the sprite to
         * that camera. Otherwise, the user is responsible to draw the sprite.
         */
        ref<Camera> camera;

    protected:
        glm::vec4 color{ 1.0f };
        /** Some sprites do not draw, like root nodes in UI elements. */
        bool isDrawable = true;
    };
}