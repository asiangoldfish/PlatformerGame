#pragma once

#include "Framework.h"

class Sprite : public FW::Entity {
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
    void addVelocity(float x, float y);
    FW::ref<FW::PhysicsComponent> getPhysicsComponent() {
        return physicsComponent;
    }

private:
    FW::ref<FW::TransformationComponent> transformationComponent;
    FW::ref<FW::DrawableComponent> drawableComponent;
    FW::ref<FW::PhysicsComponent> physicsComponent;
    FW::ref<FW::Shader> spriteShader;
};