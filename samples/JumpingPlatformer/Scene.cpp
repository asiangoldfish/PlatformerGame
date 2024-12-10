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

    glm::vec2 moveBy(0.0f);
    float speed = 100.0f;

    if (FW::Input::isKeyPressed(FW_KEY_D)) {
        moveBy.x += speed;
    }
    
    if (FW::Input::isKeyPressed(FW_KEY_A)) {
        moveBy.x -= speed;
    }
    
    if (FW::Input::isKeyPressed(FW_KEY_W)) {
        moveBy.y += speed;
    }
    
    if (FW::Input::isKeyPressed(FW_KEY_S)) {
        moveBy.y -= speed;
    }

    playerSprite->moveBy(moveBy * delta);

    FW::BaseScene::update(delta);
}
