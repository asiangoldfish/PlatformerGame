#include "JumpingPlatformerScene.h"

void JumpingPlatformerScene::init() {
    FW::BaseScene::init();
    playerSprite = FW::createRef<Sprite>();
    root->addChild(playerSprite);
    camera = FW::createRef<FW::OrthographicCamera>();
    camera->computeProjectionMatrix();
    getViewport()->setSize({ 720.0f, 720.0f });
}

void JumpingPlatformerScene::cleanUp() {}

void JumpingPlatformerScene::update(float delta) {
    camera->update(playerSprite->getShader());
    FW::BaseScene::update(delta);
}
