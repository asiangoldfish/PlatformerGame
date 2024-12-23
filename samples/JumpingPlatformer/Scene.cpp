#include "Scene.h"

void JumpingPlatformerScene::init() {
    BaseScene::init();

    playerSprite = createRef<Sprite>();
    playerSprite->setSize(50.f, 50.f);
    root->addChild(playerSprite);

    camera = createRef<OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Uncomment below to centralise the screen coordinates.
    // camera->setCentraliseScreenCoordinates(true);

    /* Physics */
    gravityForce = createRef<GravityForce>();
    mySolver.addForce(gravityForce);
}

void JumpingPlatformerScene::update(float delta) {
    camera->update(playerSprite->getShader());

    glm::vec2 moveBy(0.0f);
    float speed = 20.0f;
    float jump = 400.0f;
    static bool isJumping = false;

    // Collide with the ground floor
    if (playerSprite->getPosition().y < 100.0f) {
        playerSprite->setPosition(playerSprite->getPosition().x, 100.0f);
        playerSprite->getPhysicsComponent()->getVelocity().y = 0.0f;
        isJumping = false;
    } else {
        playerSprite->getPhysicsComponent()->getVelocity().y += -9.8f * delta;
    }

    // Go right
    if (Input::isKeyPressed(FW_KEY_D) || Input::isKeyPressed(FW_KEY_RIGHT)) {
        playerSprite->addVelocity(speed * delta, 0.0f);
    }

    // Go left
    if (Input::isKeyPressed(FW_KEY_A) || Input::isKeyPressed(FW_KEY_LEFT)) {
        playerSprite->addVelocity(-speed * delta, 0.0f);
    }
    
    // Jump
    if (Input::isKeyPressed(FW_KEY_W) && !isJumping) {
        playerSprite->addVelocity(0.0f, jump * delta);
        isJumping = true;
    }

    // Update the rest of the scene.
    BaseScene::update(delta);
}

void JumpingPlatformerScene::cleanUp() {}
