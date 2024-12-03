#include "Sprite.h"

Sprite::Sprite() {
    drawableComponent = FW::createRef<FW::DrawableComponent>();
    addComponent(drawableComponent);

    transformationComponent = FW::createRef<FW::TransformationComponent>();
    addComponent(transformationComponent);

    spriteShader = FW::createRef<FW::Shader>(
      RESOURCES_DIR + std::string("shaders/spriteShader.vs"),
      RESOURCES_DIR + std::string("shaders/spriteShader.fs"));

    // A sprite consists of a square and an image. Therefore we should add them
    // via the drawable component.
    drawableComponent->setShader(spriteShader);

    FW::ref<FW::PrimitiveQuad> quadShape = FW::createRef<FW::PrimitiveQuad>();
    drawableComponent->setShape(quadShape);

    transformationComponent->setShader(spriteShader);
}