#include "Sprite.h"

Sprite::Sprite() {
    drawableComponent = createRef<DrawableComponent>();
    addComponent(drawableComponent);

    transformationComponent = createRef<TransformationComponent>();
    addComponent(transformationComponent);

    spriteShader = createRef<Shader>(
      FW_ECS_RESOURCES_DIR + std::string("shaders/spriteShader.vs"),
      FW_ECS_RESOURCES_DIR + std::string("shaders/spriteShader.fs"));

    // A sprite consists of a square and an image. Therefore we should add them
    // via the drawable component.
    drawableComponent->setShader(spriteShader);

    ref<PrimitiveQuad> quadShape = createRef<PrimitiveQuad>();
    drawableComponent->setShape(quadShape);

    transformationComponent->setShader(spriteShader);

    physicsComponent = createRef<PhysicsComponent>();
    physicsComponent->init();
}

void Sprite::moveBy(float x, float y) {
    glm::vec2 current = transformationComponent->getPosition();
    transformationComponent->setPosition(
      { current.x + x, current.y + y, 0.0f });
}

void Sprite::setSize(float x, float y) {
    transformationComponent->setScale(x, y, 1.0f);
}

glm::vec2 Sprite::getPosition() {
    return glm::vec2(transformationComponent->getPosition().x,
                     transformationComponent->getPosition().y);
}

void Sprite::setPosition(float x, float y) {
    transformationComponent->setPosition(
      { x, y, transformationComponent->getPosition().z });
}

void Sprite::update(float delta) {
    Entity::update(delta);
    // Because the PhysicsComponent and TransformationComponent are independent
    // and know nothing about each other, we must handle position update
    // ourselves.

    auto velocity = physicsComponent->getPhysicsBody()->getVelocity();
    moveBy(velocity.x, velocity.y);
}

void Sprite::addVelocity(float x, float y) {
    physicsComponent->getPhysicsBody()->addVelocity(Vector2{ x, y });
}