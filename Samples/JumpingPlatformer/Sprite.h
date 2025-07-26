#pragma once

#include "Framework.h"

/**
 * A sprite is an squaric Entity with transformation.
 */
class Sprite : public FW::Entity {
public:
    Sprite();

    FW::ref<FW::Shader> getShader() { return spriteShader; }

    /**
     * Move by `x` and `y` units.
     *
     * Internally the method gets the current position and adds `x` and `y` to
     * it.
     */
    void moveBy(float x, float y);

    void setSize(float x, float y);

    glm::vec2 getPosition();
    void setPosition(float x, float y);

    void update(float delta) override;

    void addVelocity(float x, float y);

    FW::ref<FW::PhysicsComponent> getPhysicsComponent() {
        return physicsComponent;
    }

private:
    /**
     * The TransformationComponent is responsible for handling the sprite's
     * position, rotation, scale and more.
     */
    FW::ref<FW::TransformationComponent> transformationComponent;

    /**
     * The DrawableComponent is responsible for drawing the sprite to the
     * window.
     */
    FW::ref<FW::DrawableComponent> drawableComponent;

    /**
     * The physics component stores data like velocity, gravity and forces.
     * It is only concerned about its own data, so to make something move the
     * user must update other components like TransformationComponent or
     * DrawableComponents.
     */
    FW::ref<FW::PhysicsComponent> physicsComponent;

    /**
     * The Shader is what draws the sprite on screen. Since it is a shared
     * reference, one shader can support multiple sprites.
     */
    FW::ref<FW::Shader> spriteShader;
};