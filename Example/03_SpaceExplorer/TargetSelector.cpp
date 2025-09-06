#include "TargetSelector.h"

#include "Sprite.h"
#include "ShaderManager.h"

TargetSelector::TargetSelector(FW::ref<FW::Camera> camera) {
    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Target Selector";
    sprite->setColor(1.0f, 0.2f, 0.2f);
    sprite->setSize(150.0f);
    sprite->setPosition(0.0f, 0.0f);
    sprite->setZIndex(3);

    sprite->getComponent<FW::DrawableComponent>()->setShader("Target selector");
    sprite->getComponent<FW::TransformationComponent>()->setShader(
      "Target selector");

    entity = sprite;

    setColor(TargetSelectionState::ACTIVE);
}

void TargetSelector::update(float delta) {
    FW::SceneNode::update(delta);
}

void TargetSelector::setSelectionState(TargetSelectionState state) {
    selectionState = state;
    setColor(state);
}

void TargetSelector::setColor(TargetSelectionState type) {
    switch (type) {
        case TargetSelectionState::INACTIVE:
            color.a = 0.0f;
            break;
        case TargetSelectionState::HOVERED:
            color = glm::vec4{ 0.3f, 0.3f, 0.3f, 1.0f };
            break;
        case TargetSelectionState::ACTIVE:
            color = glm::vec4{ 1.0f, 0.2f, 0.2f, 1.0f };
            break;

        default:
            ASSERT(true, "All types not implemented yet");
            break;
    }

    entity->getComponent<FW::DrawableComponent>()->color = color;
    FW::ShaderManager::get()
      .getShader("Target selector")
      ->setParam("u_color", color);
}
