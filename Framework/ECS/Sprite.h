#pragma once

#include "Entity.h"
#include "Camera/Camera.h"

namespace FW {
    class Sprite : public Entity {
    public:
        Sprite();
        Sprite(ref<Camera> camera);
        void init();
        std::string& getShader() { return shader; }
        
        glm::vec2 getSize() { return size; }
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
        glm::vec2 position;
        glm::vec2 size{ 100.0f };
        glm::vec3 color{ 1.0f };

        /** Some sprites do not draw, like root nodes in UI elements. */
        bool isDrawable = true;
    };
}