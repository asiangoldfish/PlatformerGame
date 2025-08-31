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
}