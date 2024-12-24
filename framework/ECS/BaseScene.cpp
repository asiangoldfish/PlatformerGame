#include "BaseScene.h"

BaseScene::~BaseScene() {}

void BaseScene::init() {
    root = createRef<Entity>("root");
    viewport = createRef<Viewport>();
}

void BaseScene::update(float delta) {
    root->update(delta);
}