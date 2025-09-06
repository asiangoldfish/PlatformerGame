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
    createBackground(backgroundSprite);

    backgroundNode = FW::createRef<FW::SceneNode>();
    backgroundNode->entity = backgroundSprite;
    rootNode->addChild(backgroundNode);

    // Projectiles
    projectileRoot = FW::createRef<ProjectileRoot>();
    rootNode->addChild(projectileRoot);

    // Player ship
    playerShip = FW::createRef<PlayerShip>(camera, projectileRoot);
    playerShip->entity->getComponent<FW::DrawableComponent>()->isTransparent =
      true;
    rootNode->addChild(playerShip);

    // Enemy ship
    enemyShip = FW::createRef<EnemyShip>(camera, projectileRoot);
    enemyShip->setTargetShip(playerShip);
    rootNode->addChild(enemyShip);

    // Target selectors
    // The nodes in this root highlights objects selected by the user
    targetSelectorNode = FW::createRef<TargetSelector>();
    targetSelectorNode->camera = camera;
    rootNode->addChild(targetSelectorNode);

    // gameUI = FW::createRef<GameUI>();
    // gameUI->camera = camera;
    // gameUI->init();
    // rootNode->addChild(gameUI);
}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);

    // We wanna stick the ship to the middle of the screen, so we must also
    // move the camera.
    camera->setPosition2D(playerShip->getPosition());
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
}

void GameScene::cleanUp() {}

void GameScene::keyCallback(int key, int scancode, int action, int mods) {
    if (FW::Input::isKeyPressed(FW_KEY_SPACE)) {
        if (enemyShip->getTargetShip()) {
            enemyShip->setTargetShip(nullptr);
        } else {
            enemyShip->setTargetShip(playerShip);
        }
    }
}

void GameScene::cursorPosCallback(double xpos, double ypos) {}

void GameScene::mouseButtonCallback(int button, int action, int mods) {
    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
        // targetSelectorNode->createTarget(
        //   FW::mouseToWorld2D(FW::Input::getMouseX(),
        //                      FW::Input::getMouseY(),
        //                      1280,
        //                      720,
        //                      camera.get()));

        // Deselect player targeted ship by clicking somewhere else than the
        // targeted ship.
        // Note that the player can still select themselves without deselecting
        // the target.
        FW::ref<Ship> targetShip = playerShip->getTargetShip();
        if (targetShip && !isMouseInsideEntityBounds(targetShip, camera) &&
            !isMouseInsideEntityBounds(playerShip, camera)) {
            targetShip->setIsTargeted(false);
            playerShip->setTargetShip(nullptr);
        }

        // Ensure that if the player ship was just deselected, we won't select
        // it again.
        bool wasPlayerJustDeselected = false;

        // Player can deselect themselves by reselecting their ship
        if (playerShip->getIsTargeted() &&
            isMouseInsideEntityBounds(playerShip, camera)) {
            playerShip->setIsTargeted(false);
            wasPlayerJustDeselected = true;
        } else {

            // Target the first clicked ship
            for (auto& scene : rootNode->childNodes) {
                FW::ref<FW::Entity> entity = scene->entity;

                if (entity && isMouseInsideEntityBounds(scene, camera)) {
                    FW::ref<Ship> ship = std::dynamic_pointer_cast<Ship>(scene);

                    if (ship) {
                        // If the player just deselected themselves, make sure
                        // we don't reselect the player again.
                        // Now, set the target
                        if (!ship->getIsTargeted()) {
                            ship->setIsTargeted(true);

                            if (ship != playerShip) {
                                playerShip->setTargetShip(ship);
                            }
                        }
                        break;
                    }
                }
            }
        }
    }
}
