#include "Scene.h"

void JumpingPlatformerScene::init() {
    BaseScene::init();

    // The physics engine is a component of the scene.
    physicsEngine = createScope<PhysicsEngine>();

    // This creates a reference to the sprite. A sprite has a physics component
    // with a physics body. The physics engine updates the physics body's
    // positions, which we synchronize with the sprite's position in the
    // transformation vector.
    playerSprite = createRef<Sprite>();
    playerSprite->setSize(50.f, 50.f);

    // We must specify whether the
    physicsEngine->addPhysicsBody(
      playerSprite->getPhysicsComponent()->getPhysicsBody());

    // The physics body is by default static. We can make it dynamic, so forces
    // like gravity affects it.
    playerSprite->getPhysicsComponent()->getPhysicsBody()->isDynamic = true;

    root->addChild(playerSprite);

    camera = createRef<OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);
    // Uncomment below to centralise the screen coordinates.
    // camera->setCentraliseScreenCoordinates(true);
}

void JumpingPlatformerScene::update(float delta) {
    // Step 1: Update the physics
    physicsEngine->update(delta);

    // We must update all entities' positions based on the physics simulation
    Vector2 pos2d = playerSprite->getPhysicsComponent()->getPhysicsBody()->position;
    Vector2 newPos = playerSprite->getPhysicsComponent()->getPhysicsBody()->velocity + pos2d; 
    Vector3 pos3d = Vector3(newPos, 0.0f);
    playerSprite->getTransformationComponent()->setPosition(pos3d);
    playerSprite->getPhysicsComponent()->getPhysicsBody()->position = newPos;
    
    // Step 2: Update the visuals
    BaseScene::update(delta);

    // Step 3: Update the logic
    camera->update(playerSprite->getShader());

    glm::vec2 moveBy(0.0f);
    float speed = 20.0f;
    float jump = 40.0f;
    static bool isJumping = false;

    // Collide with the ground floor
    // if (playerSprite->getPosition().y < 300.0f) {
    //     playerSprite->setPosition(playerSprite->getPosition().x, 300.0f);
    //     playerSprite->getPhysicsComponent()->getVelocity().y = 0.0f;
    //     isJumping = false;
    // } else {
    //     // playerSprite->getPhysicsComponent()->getVelocity().y += -9.8f *
    //     // delta;
    // }

    // Go right
    if (Input::isKeyPressed(FW_KEY_D) || Input::isKeyPressed(FW_KEY_RIGHT)) {
        playerSprite->addVelocity(speed * delta, 0.0f);
    }

    // Go left
    if (Input::isKeyPressed(FW_KEY_A) || Input::isKeyPressed(FW_KEY_LEFT)) {
        playerSprite->addVelocity(-speed * delta, 0.0f);
    }

    // Jump
    if (Input::isKeyPressed(FW_KEY_W)) {// && !isJumping) {
        playerSprite->addVelocity(0.0f, jump * delta);
        isJumping = true;
    }
}

void JumpingPlatformerScene::cleanUp() {}
