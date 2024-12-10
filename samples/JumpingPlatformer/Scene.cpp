#include "Scene.h"

void JumpingPlatformerScene::init() {
    FW::BaseScene::init();

    playerSprite = FW::createRef<Sprite>();
    playerSprite->setSize(300.f, 300.f);
    root->addChild(playerSprite);

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Uncomment below to centralise the screen coordinates.
    // camera->setCentraliseScreenCoordinates(true);
}

void JumpingPlatformerScene::cleanUp() {}

void JumpingPlatformerScene::update(float delta) {
    camera->update(playerSprite->getShader());
    FW::BaseScene::update(delta);
}
