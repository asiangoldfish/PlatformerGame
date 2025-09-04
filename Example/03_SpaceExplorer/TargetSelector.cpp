#include "TargetSelector.h"

#include "Sprite.h"
#include "ShaderManager.h"

TargetSelector::TargetSelector() {
    FW::ShaderManager::get().createShaderFromFiles(
      "Target selector",
      SHADERS_DIR + std::string("ECS_sprite.vs"),
      SHADERS_DIR + std::string("target_selector.fs"));

    FW::ShaderManager::get()
      .getShader("Target selector")
      ->setParam("u_color", glm::vec4(1.0f, 0.1f, 0.2f, 1.0f));
}

void TargetSelector::update(float delta) {
    FW::SceneNode::update(delta);
}

void TargetSelector::createTarget(glm::vec2 position) {
    FW::ref<FW::SceneNode> scene = FW::createRef<FW::SceneNode>();

    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Target Selector";
    sprite->setColor(1.0f, 0.2f, 0.2f);
    sprite->setSize(200.f);
    sprite->setPosition(position);
    sprite->setZIndex(3);

    sprite->getComponent<FW::DrawableComponent>()->setShader("Target selector");
    sprite->getComponent<FW::TransformationComponent>()->setShader(
      "Target selector");

    scene->entity = sprite;
    addChild(scene);
}
