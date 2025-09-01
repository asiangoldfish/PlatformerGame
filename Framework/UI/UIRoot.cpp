#include "UIRoot.h"
#include "ShaderManager.h"

namespace FW::UI {
    void UIRoot::init() {
        shader = "inbuilt_ecs_shader";

        // If performance becomes an issue, then find some way to avoiid trying
        // to create a new shader every time we initialise a new UI element.
        ShaderManager::get().createShaderFromFiles(
          shader,
          SHADERS_DIR + std::string("ECS_sprite.vs"),
          SHADERS_DIR + std::string("ECS_sprite.fs"));

        transformationComponent = FW::createRef<FW::TransformationComponent>();
        addComponent(transformationComponent);
        transformationComponent->setShader(shader);
    }

    void UIRoot::setPosition(const glm::vec2& position) {
        glm::vec2 delta = this->position - position;
        this->position = position;

        // For every child node we must add their position with our delta.
        for (auto& childNode : getChildren()) {
            // Transform component was for some reason not found in the
            // child node (??) Probably not an assert, but this is a strange
            // behaviour.
            auto transformComponent =
              childNode->getComponent<FW::ref<FW::TransformationComponent>>();

            if (transformComponent) {
                glm::vec3 newPosition = transformationComponent->getPosition() +
                                        glm::vec3{ delta, 0.0f };
                transformationComponent->setPosition(newPosition);
            }
        }
    }
}