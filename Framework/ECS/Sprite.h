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
        void update(Camera* camera, float delta);

    protected:
        ref<TransformationComponent> transformationComponent;
        ref<DrawableComponent> drawableComponent;
        ref<Shader> spriteShader;
    };
}