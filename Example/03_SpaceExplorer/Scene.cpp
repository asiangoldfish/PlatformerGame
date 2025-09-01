#include "Scene.h"

/** Returns angle in radian in z-axis */
glm::vec3 getRotationWithMouse() {
    double x = static_cast<double>(FW::Input::getMouseX());
    double xRatioWithScreen = (x / 640.0) - 1.0;
    double y = static_cast<double>(FW::Input::getMouseY());
    double yRatioWithScreen = (y / 360.0) - 1.0;

    return glm::vec3{ 0.f, 0.f, atan2(yRatioWithScreen, xRatioWithScreen) };
}

FW::ref<Bullet> createBullet(FW::ref<FW::Camera> camera) {
    FW::ref<Bullet> bullet = FW::createRef<Bullet>();

    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Bullet";
    sprite->setColor(1.0f, 0.3f, 0.75f);
    sprite->setSize(30, 10);
    sprite->setPosition(0.0f, 0.0f);

    bullet->entity = sprite;

    return bullet;
}

void GameScene::init() {
    FW::BaseScene::init();

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);

    playerShip = FW::createRef<Ship>(camera);
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
    playerShip->setPosition(FW::Input::getMouseX(),
                            -FW::Input::getMouseY() + 720.0f);

    playerShip->setRotation(-getRotationWithMouse());
}

void GameScene::cleanUp() {}

void GameScene::keyCallback(int key, int scancode, int action, int mods) {}
void GameScene::cursorPosCallback(double xpos, double ypos) {}

void GameScene::mouseButtonCallback(int button, int action, int mods) {
    // Shoot bullet! Algorithm:
    // 1. Find player position
    // 2. Spawn bullet and send it with velocity vector
    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
        glm::vec2 playerPos = playerShip->getPosition();
        float angle = -getRotationWithMouse().z;
        float speed = 600.0f;

        auto bullet = createBullet(camera);
        bullet->setPosition(playerPos);
        bullet->velocity = glm::vec2{ cos(angle) * speed, sin(angle) * speed };
        bullet->setRotation(glm::vec3{0.0f, 0.0f, angle});
        projectileRoot->addChild(bullet);
    }
}
