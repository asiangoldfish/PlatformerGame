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

    // Background - Must be drawn first
    auto backgroundSprite = FW::createRef<FW::Sprite>(camera);
    backgroundSprite->name = "background";
    backgroundSprite->setSize(2000.0f);
    backgroundSprite->setTexture(
      "background", TEXTURES_DIR + std::string("space_background.jpg"));

    backgroundNode = FW::createRef<FW::SceneNode>();
    backgroundNode->entity = backgroundSprite;
    rootNode->addChild(backgroundNode);

    // Player ship
    playerShip = FW::createRef<Ship>(camera);
    playerShip->entity->getComponent<FW::DrawableComponent>()->isTransparent =
      true;
    rootNode->addChild(playerShip);

    // Projectiles
    projectileRoot = FW::createRef<ProjectileRoot>();
    rootNode->addChild(projectileRoot);

    // gameUI = FW::createRef<GameUI>();
    // gameUI->camera = camera;
    // gameUI->init();
    // rootNode->addChild(gameUI);
}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);

    playerShip->setRotation(-getRotationWithMouse());

    // We wanna stick the ship to the middle of the screen, so we must also
    // move the camera.
    glm::vec2 camPos = camera->getPosition2D();
    camera->setPosition2D(playerShip->getPosition());
    backgroundNode->entity->getComponent<FW::TransformationComponent>()
      ->setPosition(-playerShip->getPosition() * parallaxFactor);

    // Shoot bullet! Algorithm:
    // 1. Find player position
    // 2. Spawn bullet and send it with velocity vector
    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
        playerShip->fireBullets(projectileRoot);
    }

    renderSystem.draw(rootNode);
}

void GameScene::cleanUp() {}
void GameScene::keyCallback(int key, int scancode, int action, int mods) {}
void GameScene::cursorPosCallback(double xpos, double ypos) {}
void GameScene::mouseButtonCallback(int button, int action, int mods) {}
