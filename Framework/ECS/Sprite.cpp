#include "Sprite.h"

namespace FW {
    Sprite::Sprite() {
        init();
    }

    Sprite::Sprite(ref<Camera> camera) {
        this->camera = camera;
        init();
    }

    void Sprite::init() {
        spriteShader =
          FW::createRef<FW::Shader>(SHADERS_DIR + std::string("ECS_sprite.vs"),
                                    SHADERS_DIR + std::string("ECS_sprite.fs"));
        ASSERT(spriteShader, "Failed to create SpriteShader");
        
        transformationComponent = FW::createRef<FW::TransformationComponent>();
        addComponent(transformationComponent);

        if (isDrawable) {
            drawableComponent = FW::createRef<FW::DrawableComponent>();
            addComponent(drawableComponent);
            drawableComponent->setShader(spriteShader);
            drawableComponent->setShape(FW::createRef<FW::PrimitiveQuad>());
        }

        transformationComponent->setShader(spriteShader);
    }

    void Sprite::moveBy(float x, float y) {
        glm::vec2 current = transformationComponent->getPosition();
        transformationComponent->setPosition(
          { current.x + x, current.y + y, 0.0f });
    }

    void Sprite::setSize(float x, float y) {
        transformationComponent->setScale(x, y, 1.0f);
        size.x = x;
        size.y = y;

        for (auto& childNode : getChildren()) {
            if (Sprite* childSprite = dynamic_cast<Sprite*>(childNode.get());
                childSprite != nullptr) {
                glm::vec2 sizeRatio = glm::vec2{ x, y } / size;

                glm::vec2 newSize = childSprite->getSize() * sizeRatio;
                childSprite->setSize(newSize);
            }
        }
    }

    void Sprite::setSize(float uniformSize) {
        setSize(uniformSize, uniformSize);
    }

    void Sprite::setSize(glm::vec2 size) {
        setSize(size.x, size.y);
    }

    glm::vec2 Sprite::getPosition() {
        return glm::vec2(transformationComponent->getPosition().x,
                         transformationComponent->getPosition().y);
    }

    void Sprite::setPosition(float x, float y) {
        transformationComponent->setPosition(
          { x, y, transformationComponent->getPosition().z });
        position.x = x;
        position.y = y;

        for (auto& childNode : getChildren()) {
            if (Sprite* childSprite = dynamic_cast<Sprite*>(childNode.get());
                childSprite != nullptr) {
                glm::vec2 positionOffset =
                  position - childSprite->getPosition();
                childSprite->setPosition(childSprite->getPosition() +
                                         positionOffset);
            }
        }
    }

    void Sprite::setPosition(glm::vec2 position) {
        setPosition(position.x, position.y);
    }

    void Sprite::setRotation(float yaw, float pitch, float roll) {
        transformationComponent->setRotation(yaw, pitch, roll);
    }

    void Sprite::setRotation(glm::vec3 rot) {
        transformationComponent->setRotation(rot.x, rot.y, rot.z);
    }

    void Sprite::update(float delta) {
        if (isDrawable && camera) {
            camera->update(drawableComponent->getShader());
        }

        Entity::update(delta);
    }
}