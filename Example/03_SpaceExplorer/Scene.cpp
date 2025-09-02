#include "Scene.h"

#include "SpaceExplorerUtils.h"

void GameScene::init() {
    FW::BaseScene::init();

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Centralising the screen coordinates makes sure that the origin is always
    // in the middle of the screen, thus allowing us to follow the player by
    // simply setting it to the player's position.
    camera->setCentraliseScreenCoordinates(true);

    playerShip = FW::createRef<Ship>(camera);
    playerShip->setPosition(0.0f, 0.0f, -3.f);
    rootNode->addChild(playerShip);

    projectileRoot = FW::createRef<ProjectileRoot>();
    rootNode->addChild(projectileRoot);

    // gameUI = FW::createRef<GameUI>();
    // gameUI->camera = camera;
    // gameUI->init();
    // rootNode->addChild(gameUI);
}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);
    // playerShip->setPosition(FW::Input::getMouseX(),
    //                         -FW::Input::getMouseY() + 720.0f);

    playerShip->setRotation(-getRotationWithMouse());

    // We wanna stick the ship to the middle of the screen, so we must also
    // move the camera.
    glm::vec2 camPos = camera->getPosition2D();
    camera->setPosition2D(playerShip->getPosition());

    // Shoot bullet! Algorithm:
    // 1. Find player position
    // 2. Spawn bullet and send it with velocity vector
    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
        playerShip->fireBullets(projectileRoot);
    }
}

void GameScene::cleanUp() {}
void GameScene::keyCallback(int key, int scancode, int action, int mods) {}
void GameScene::cursorPosCallback(double xpos, double ypos) {}
void GameScene::mouseButtonCallback(int button, int action, int mods) {}
