#include "Scene.h"

void JumpingPlatformerScene::init() {
    // Initialize the viewport and the scene tree.
    FW::BaseScene::init();

    // The player controlled sprite. This is appended to the scene tree.
    playerSprite = FW::createRef<Sprite>();
    playerSprite->setSize(50.f, 50.f);
    root->addChild(playerSprite);

    // For 2D scenes, the orthographic camera is appropriate. The camera size
    // is usually equal to the window size.
    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Uncomment below to centralise the screen coordinates.
    // camera->setCentraliseScreenCoordinates(true);

    // Add physics components here.
    gravityForce = FW::createRef<FW::Physics::GravityForce>();
    mySolver.addForce(gravityForce);
}

void JumpingPlatformerScene::update(float delta) {
    // Easy way to update camera position, etc. If the camera is still, then
    // this can be omitted for performance.
    camera->update(playerSprite->getShader());

    glm::vec2 moveBy(0.0f);
    float speed = 300.0f;
    float jump = 400.0f;
    static bool isJumping = false;

    playerSprite->getPhysicsComponent()->setVelocity(
      0.0f, playerSprite->getPhysicsComponent()->getVelocity().y, 0.0f);

    // Collide with the ground floor
    if (playerSprite->getPosition().y < 100.0f) {
        playerSprite->setPosition(playerSprite->getPosition().x, 100.0f);
        playerSprite->getPhysicsComponent()->getVelocity().y = 0.0f;
        isJumping = false;
    } else {
        playerSprite->getPhysicsComponent()->getVelocity().y += -9.8f * delta;
    }

    // Go right
    if (FW::Input::isKeyPressed(FW_KEY_D) == GLFW_PRESS || FW::Input::isKeyPressed(FW_KEY_LEFT) == GLFW_PRESS) {
        playerSprite->addVelocity(speed * delta, 0.0f);
    }

    // Go left
    if (FW::Input::isKeyPressed(FW_KEY_A) == GLFW_PRESS ||
        FW::Input::isKeyPressed(FW_KEY_LEFT) == GLFW_PRESS) {
        playerSprite->addVelocity(-speed * delta, 0.0f);
    }

    // Jump
    if (FW::Input::isKeyPressed(FW_KEY_W) && !isJumping) {
        playerSprite->addVelocity(0.0f, jump * delta);
        isJumping = true;
    }

    // Update the rest of the scene.
    FW::BaseScene::update(delta);
}

void JumpingPlatformerScene::cleanUp() {}
