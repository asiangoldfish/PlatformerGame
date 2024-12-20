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

void Sprite::moveBy(glm::vec2 moveBy) {
    glm::vec2 current = transformationComponent->getPosition();
    transformationComponent->setPosition({ 
        current.x + moveBy.x, current.y + moveBy.y, 0.0f });
}

void Sprite::moveBy(float x, float y) {
    moveBy(glm::vec2(x, y));
}

void Sprite::setSize(float x, float y) {
    transformationComponent->setScale(x, y, 1.0f);
}

glm::vec2 Sprite::getPosition() {
    return glm::vec2(
        transformationComponent->getPosition().x,
        transformationComponent->getPosition().y
    );
}

void Sprite::setPosition(float x, float y) {
    transformationComponent->setPosition(
        { x, y, transformationComponent->getPosition().z });
}
