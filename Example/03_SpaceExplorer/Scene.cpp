#include "Scene.h"

glm::vec3 getRotationWithMouse() {
    double x = static_cast<double>(FW::Input::getMouseX());
    double xRatioWithScreen = (x / 640.0) - 1.0;
    double y = static_cast<double>(FW::Input::getMouseY());
    double yRatioWithScreen = (y / 360.0) - 1.0;

    return glm::vec3{ 0.f, 0.f, atan2(yRatioWithScreen, xRatioWithScreen) };
}

void GameScene::init() {
    FW::BaseScene::init();

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);

    playerShip = FW::createRef<Ship>();
    playerShip->sprite->setCamera(camera);
    root->addChild(playerShip->sprite);

    gameUI.camera = camera;
    gameUI.init();
    root->addChild(gameUI.uiRoot);

    // TODO figure out why space ship sometimes renders, sometimes doesn't. Race
    // condition??
}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);
    playerShip->setPosition(FW::Input::getMouseX(),
                            -FW::Input::getMouseY() + 720.0f);
}

void GameScene::cleanUp() {}

void GameScene::keyCallback(int key, int scancode, int action, int mods) {}
void GameScene::cursorPosCallback(double xpos, double ypos) {}
void GameScene::mouseButtonCallback(int button, int action, int mods) {
    // TODO move to game loop so it is called continuously
    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
        playerShip->setRotation(-getRotationWithMouse());
    }
}