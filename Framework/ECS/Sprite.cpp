#include "Sprite.h"

namespace FW {
    Sprite::Sprite() {
        drawableComponent = FW::createRef<FW::DrawableComponent>();
        addComponent(drawableComponent);

        transformationComponent = FW::createRef<FW::TransformationComponent>();
        addComponent(transformationComponent);

        spriteShader =
          FW::createRef<FW::Shader>(SHADERS_DIR + std::string("sprite.vs"),
                                    SHADERS_DIR + std::string("sprite.fs"));

        drawableComponent->setShader(spriteShader);

        FW::ref<FW::PrimitiveQuad> quadShape =
          FW::createRef<FW::PrimitiveQuad>();
        drawableComponent->setShape(quadShape);

        transformationComponent->setShader(spriteShader);
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

    void Sprite::setRotation(float yaw, float pitch, float roll) {
        transformationComponent->setRotation(yaw, pitch, roll);
    }

    void Sprite::setRotation(glm::vec3 rot) {
        transformationComponent->setRotation(rot.x, rot.y, rot.z);
    }

    void Sprite::update(float delta) {
        Entity::update(delta);
    }
    
    void Sprite::update(Camera* camera, float delta) {
        update(delta);
        camera->update(spriteShader);
    }
}