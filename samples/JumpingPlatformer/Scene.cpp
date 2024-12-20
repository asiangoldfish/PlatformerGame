#include "Scene.h"

void JumpingPlatformerScene::init() {
    FW::BaseScene::init();

    playerSprite = FW::createRef<Sprite>();
    playerSprite->setSize(50.f, 50.f);
    root->addChild(playerSprite);

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Uncomment below to centralise the screen coordinates.
    // camera->setCentraliseScreenCoordinates(true);

    /* Physics */
    gravityForce = FW::createRef<FW::Physics::GravityForce>();
    mySolver.addForce(gravityForce);
}

void JumpingPlatformerScene::update(float delta) {
    camera->update(playerSprite->getShader());

    // gravitational acceleration
    float acc = -980.f;

    glm::vec2 moveBy(0.0f);
    float speed = 3000.0f;

    if (FW::Input::isKeyPressed(FW_KEY_D)) {
        moveBy.x += speed;
    }
    
    if (FW::Input::isKeyPressed(FW_KEY_A)) {
        moveBy.x -= speed;
    }
    
    if (FW::Input::isKeyPressed(FW_KEY_W)) {
        moveBy.y += speed;
    }
    
    moveBy.y += acc;

    playerSprite->moveBy(moveBy * delta);

    if (playerSprite->getPosition().y < 100.0f) {
        playerSprite->setPosition(playerSprite->getPosition().x, 100.0f);
    }

    FW::BaseScene::update(delta);
}

void JumpingPlatformerScene::cleanUp() {}
