#pragma once

#include "Framework.h"

/**
 * A sprite is an squaric Entity with transformation.
 */
class Sprite : public FW::Entity {
public:
    Sprite();

    FW::ref<FW::Shader> getShader() { return spriteShader; }

public: // Transformations
    /**
     * Move by `x` and `y` units.
     *
     * Internally the method gets the current position and adds `x` and `y` to
     * it.
     */
    void moveBy(glm::vec2 moveBy);
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
    FW::ref<FW::TransformationComponent> transformationComponent;
    FW::ref<FW::DrawableComponent> drawableComponent;
    FW::ref<FW::PhysicsComponent> physicsComponent;

    // TODO add physics component
    FW::ref<FW::Shader> spriteShader;
};