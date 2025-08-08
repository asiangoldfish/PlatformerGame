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

    playerSprite = FW::createRef<Sprite>();
    playerSprite->setSize(500, 250);
    playerSprite->setPosition(600.f, 400.f);
    root->addChild(playerSprite);
}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);

    camera->update(playerSprite->getShader());

    auto rot = getRotationWithMouse();
    playerSprite->setRotation(-rot);
}

void GameScene::cleanUp() {}
