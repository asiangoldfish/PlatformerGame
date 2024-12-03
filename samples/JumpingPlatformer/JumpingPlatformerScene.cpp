#include "JumpingPlatformerScene.h"

void JumpingPlatformerScene::init() {
    FW::BaseScene::init();
    playerSprite = FW::createRef<Sprite>();
    root->addChild(playerSprite);
    camera = FW::createRef<FW::OrthographicCamera>();
    camera->getFrustum().right = 1.0f;
    camera->getFrustum().top = 1.0f;
    camera->computeProjectionMatrix();
}

void JumpingPlatformerScene::cleanUp() {}

void JumpingPlatformerScene::update(float delta) {
    glViewport(0,0,720,720);
    camera->update(playerSprite->getShader());
    FW::BaseScene::update(delta);
}
