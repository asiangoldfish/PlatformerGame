#include "Scene.h"

#include "SpaceExplorerUtils.h"
#include "ShaderManager.h"
#include "Util.h"

void createBackground(FW::ref<FW::Sprite> backgroundSprite) {
    backgroundSprite->name = "background";
    backgroundSprite->setSize(1920.0f);

    std::string bgShaderName = "stars_background_01";

    FW::ShaderManager::get().createShaderFromFiles(
      bgShaderName,
      SHADERS_DIR + std::string("ECS_sprite.vs"),
      SHADERS_DIR + std::string("procedural_stars.fs"));

    backgroundSprite->getComponent<FW::DrawableComponent>()->setShader(
      bgShaderName);
    backgroundSprite->getComponent<FW::TransformationComponent>()->setShader(
      bgShaderName);
}

void GameScene::setDebugging(FW::ref<Debugging> d) {
    debugging = d;

    // Debugging windows
    debugging->playerShip = playerShip;
}

void GameScene::init() {
    FW::BaseScene::init();

    FW::ShaderManager::get().createShaderFromFiles(
      "Target selector",
      SHADERS_DIR + std::string("ECS_sprite.vs"),
      SHADERS_DIR + std::string("target_selector.fs"));

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Centralising the screen coordinates makes sure that the origin is always
    // in the middle of the screen, thus allowing us to follow the player by
    // simply setting it to the player's position.
    camera->setCentraliseScreenCoordinates(true);

    // Background - Must be drawn first
    auto backgroundSprite = FW::createRef<FW::Sprite>(camera);
    createBackground(backgroundSprite);

    backgroundNode = FW::createRef<FW::SceneNode>();
    backgroundNode->entity = backgroundSprite;
    rootNode->addChild(backgroundNode);

    // Projectiles
    projectileRoot = FW::createRef<ProjectileRoot>();
    rootNode->addChild(projectileRoot);

    // Player ship
    playerShip = FW::createRef<PlayerShip>(camera, projectileRoot);
    playerShip->setPosition(-200.0f, 300.0f);
    rootNode->addChild(playerShip);

    // Enemy ships
    enemyShips = FW::createRef<ShipCollection>();
    rootNode->addChild(enemyShips);

    FW::ref<EnemyShip> enemyShip =
      FW::createRef<EnemyShip>(camera, projectileRoot);
    enemyShip->setTargetShip(playerShip);
    enemyShips->addChild(enemyShip);

    // Systems
    selectionSystem.setCamera(camera);
    selectionSystem.setPlayerShip(playerShip);
    selectionSystem.setEnemiesRoot(enemyShips);

    // Test minable asteroid
    asteroid = FW::createRef<Asteroid>(camera);
    rootNode->addChild(asteroid);

    // gameUI = FW::createRef<GameUI>();
    // gameUI->camera = camera;
    // gameUI->init();
    // rootNode->addChild(gameUI);
}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);
    backgroundNode->entity->getComponent<FW::TransformationComponent>()
      ->setPosition(playerShip->getPosition());

    // Scroll the background with the player movement
    // backgroundNode->entity->getComponent<FW::TransformationComponent>()
    //   ->setPosition(-playerShip->getPosition() * parallaxFactor);
    FW::Shader* bgShader =
      FW::ShaderManager::get().getShader("stars_background_01");
    glm::vec2 uploadVec = camera->getPosition2D();
    uploadVec.x = uploadVec.y;
    uploadVec.y = camera->getPosition2D().x;
    bgShader->setParam("u_camera", uploadVec);

    renderSystem.draw(rootNode);

    if (debugging) {
        debugging->draw(delta);
    }

    enemyShips->update(delta);
}

void GameScene::cleanUp() {}

void GameScene::keyCallback(int key, int scancode, int action, int mods) {}

void GameScene::cursorPosCallback(double xpos, double ypos) {
    selectionSystem.onHovering();
}

void GameScene::mouseButtonCallback(int button, int action, int mods) {
    selectionSystem.onClicking();
}
