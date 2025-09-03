#include "Sprite.h"
#include "ShaderManager.h"

namespace FW {
    Sprite::Sprite() {
        init();
    }

    Sprite::Sprite(ref<Camera> camera) {
        this->camera = camera;
        init();
    }

    void Sprite::init() {
        shader = "inbuilt_ecs_shader";
        ShaderManager::get().createShaderFromFiles(shader, SHADERS_DIR + std::string("ECS_sprite.vs"),
                                    SHADERS_DIR + std::string("ECS_sprite.fs"));

        transformationComponent = FW::createRef<FW::TransformationComponent>();
        addComponent(transformationComponent);

        if (isDrawable) {
            drawableComponent = FW::createRef<FW::DrawableComponent>();
            addComponent(drawableComponent);
            drawableComponent->setShader(shader);
            drawableComponent->setShape(FW::createRef<FW::PrimitiveQuad>());
        }

        transformationComponent->setShader(shader);
    }

    void Sprite::moveBy(float x, float y) {
        glm::vec2 current = transformationComponent->getPosition();
        transformationComponent->setPosition(
          { current.x + x, current.y + y, 0.0f });
    }

    glm::vec2 Sprite::getSize() {
        return glm::vec2{ transformationComponent->getScale().x,
                          transformationComponent->getScale().y };
    }

    void Sprite::setSize(float x, float y) {
        // auto size = glm::vec2{ transformationComponent->getScale().x,
                            //    transformationComponent->getScale().y };
        transformationComponent->setScale(x, y, 1.0f);

        // for (auto& childNode : getChildren()) {
        //     if (Sprite* childSprite = dynamic_cast<Sprite*>(childNode.get());
        //         childSprite != nullptr) {
        //         glm::vec2 sizeRatio = glm::vec2{ x, y } / size;

        //         glm::vec2 newSize = childSprite->getSize() * sizeRatio;
        //         childSprite->setSize(newSize);
        //     }
        // }
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
        // auto position = glm::vec2(transformationComponent->getPosition().x,
        //   transformationComponent->getPosition().y);
        transformationComponent->setPosition(
          { x, y, transformationComponent->getPosition().z });

        // for (auto& childNode : getChildren()) {
        //     if (Sprite* childSprite = dynamic_cast<Sprite*>(childNode.get());
        //         childSprite != nullptr) {
        //         glm::vec2 positionOffset =
        //           position - childSprite->getPosition();
        //         childSprite->setPosition(childSprite->getPosition() +
        //                                  positionOffset);
        //     }
        // }
    }

    void Sprite::setPosition(float x, float y, float z) {
        transformationComponent->setPosition({ x, y, z });
    }

    void Sprite::setPosition(glm::vec2 position) {
        setPosition(position.x, position.y);
    }

    void Sprite::setPosition(glm::vec3 position) {
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
            Shader* shaderRef = ShaderManager::get().bind(drawableComponent->getShader());
            camera->update(shaderRef);
        }

        Entity::update(delta);
    }
    void Sprite::setTexture(const std::string& name,
                            const std::string filepath) {
        drawableComponent->setTexture(name, filepath);
    }
    void Sprite::setZIndex(uint32_t z) {
        if (drawableComponent) {
            drawableComponent->Z_index = z;
        }
    }
    void Sprite::setIsTransparent(const bool b) {
        if (drawableComponent) {
            drawableComponent->isTransparent = b;
        }
    }
}