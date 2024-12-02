#include "BaseScene.h"

FW::BaseScene::~BaseScene() {
}

void FW::BaseScene::init() {
    root = createRef<Entity>();
    root->name = "root";
}

void FW::BaseScene::update(float delta) {
    root->update(delta);
}