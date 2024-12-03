#include "JumpingPlatformerScene.h"

JumpingPlatformerScene::JumpingPlatformerScene() {}

JumpingPlatformerScene::~JumpingPlatformerScene() {}

void JumpingPlatformerScene::init() {
    FW::BaseScene::init();
    drawableEntity = FW::createRef<Sprite>();
    root->addChild(drawableEntity);
}

void JumpingPlatformerScene::cleanUp() {}

void JumpingPlatformerScene::update(float delta) {
    FW::BaseScene::update(delta);
}
