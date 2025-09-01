#include "BaseScene.h"

FW::BaseScene::~BaseScene() {
}

void FW::BaseScene::init() {
    rootNode = createRef<SceneNode>();
    rootNode->entity = createRef<Entity>();
    rootNode->entity->name = "root";
    viewport = FW::createRef<FW::Viewport>();
}

void FW::BaseScene::update(float delta) {
    if (rootNode) {
        rootNode->update(delta);
    }
}

void FW::SceneNode::update(float delta) {
    if (entity) {
        entity->update(delta);
    }

    for (auto& childNode : childNodes) {
        childNode->update(delta);
    }
}

void FW::SceneNode::addChild(ref<SceneNode> node) {
    childNodes.push_back(node);
}
