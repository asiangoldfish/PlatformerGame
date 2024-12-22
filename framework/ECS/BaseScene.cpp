#include "BaseScene.h"

FW::BaseScene::~BaseScene() {}

void FW::BaseScene::init() {
    root = createRef<Entity>();
    root->name = "root";
    viewport = FW::createRef<FW::Viewport>();
}

void FW::BaseScene::update(float delta) {
    root->update(delta);
}