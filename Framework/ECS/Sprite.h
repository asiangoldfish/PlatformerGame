#pragma once

#include "Entity.h"
#include "Camera/Camera.h"

namespace FW {
    class Sprite : public Entity {
    public:
        Sprite();
        FW::ref<FW::Shader> getShader() { return spriteShader; }
        void moveBy(float x, float y);
        void setSize(float x, float y);
        glm::vec2 getPosition();
        void setPosition(float x, float y);
        void setRotation(float yaw, float pitch, float roll);
        void setRotation(glm::vec3 rot);
        void update(float delta) override;
        void setCamera(ref<Camera> camera) { this->camera = camera; }

    protected:
        ref<TransformationComponent> transformationComponent;
        ref<DrawableComponent> drawableComponent;
        ref<Shader> spriteShader;

        /**
         * If the camera exists, then Sprite::draw() will draw the sprite to
         * that camera. Otherwise, the user is responsible to draw the sprite.
         */
        ref<Camera> camera;
    };
}