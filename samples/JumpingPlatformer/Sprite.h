#pragma once

#include "Framework.h"

/**
 * A sprite is an squaric Entity with transformation.
 */
class Sprite : public FW::Entity {
public:
    Sprite();

private:
    FW::ref<FW::TransformationComponent> transformationComponent;
    FW::ref<FW::DrawableComponent> drawableComponent;

    // TODO add physics component
    FW::ref<FW::Shader> spriteShader;
};