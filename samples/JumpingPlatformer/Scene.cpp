#include "Scene.h"

Vector2 extrapolatePosition(Vector2 position,
                            Vector2 velocity,
                            Vector2 acceleration,
                            float alpha,
                            float timeStep) {
    return position + velocity * (alpha * timeStep) +
           0.5f * acceleration * (alpha * timeStep) * (alpha * timeStep);
    ;
}

void JumpingPlatformerScene::init() {
    BaseScene::init();

    // The physics engine is a component of the scene.
    physicsEngine = createScope<PhysicsEngine>();

    // This creates a reference to the sprite. A sprite has a physics component
    // with a physics body. The physics engine updates the physics body's
    // positions, which we synchronize with the sprite's position in the
    // transformation vector.
    playerSprite = createRef<Sprite>("Player");
    playerSprite->setSize(50.f, 50.f);
    playerSprite->getPhysicsComponent()->getPhysicsBody()->setPosition(
      Vector2{ 500.0f, 700.0f });

    // Let the physics engine know about the player sprite's physics body.
    physicsEngine->addPhysicsBody(
      playerSprite->getPhysicsComponent()->getPhysicsBody());

    // The physics body is by default static. We can make it dynamic, so forces
    // like gravity affects it.
    playerSprite->getPhysicsComponent()->getPhysicsBody()->isDynamic = true;

    // This creates the ground sprite.
    ground = createRef<Sprite>("Ground");
    ground->setSize(500.0f, 50.0f);
    // By default, a physics body is static.
    ground->getPhysicsComponent()->getPhysicsBody()->isDynamic = false;
    physicsEngine->addPhysicsBody(
      ground->getPhysicsComponent()->getPhysicsBody());

    // Add to sprites collection.
    // The benefit with using heap allocated sprites is they are not recreated
    // and copied when append them to the collection.
    sprites.push_back(playerSprite);
    sprites.push_back(ground);

    // Unless you deterministically append the sprites in order of dynamicity,
    // we should sort them. This helps the CPU's branch prediction to guess
    // currectly when we later decide whether we should copy the physics
    // transformation to the render transformation.
    std::sort(sprites.begin(), sprites.end(), [](ref<Sprite> a, ref<Sprite> b) {
        return a->getPhysicsComponent()->getPhysicsBody()->isDynamic >
               b->getPhysicsComponent()->getPhysicsBody()->isDynamic;
    });

    // The scene is a tree data structure. It consists of n number of children.
    // To render an entity, it must be registered to the tree in some way.
    // Note that a sprite doesn't need to be in the scene to be physics
    // simulated. These subsystems work independently.
    root->addChild(playerSprite);
    root->addChild(ground);

    // Instantiate the camera. For 2d games, the camera size should be the size
    // of the viewport. This is usually the same as the game window (for
    // fullscreen games, this is usually the same as the monitor resolution.)
    camera = createRef<OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);

    // By default, the screen coordinate (0, 0) is at the bottom left of the
    // viewport. This can be centralised, so (0, 0) is in the middle of the
    // monitor. Uncomment below to centralise the screen coordinates.
    // camera->setCentraliseScreenCoordinates(true);
}

/*
  If the Scene relies on physics, the Engine uses the following procedure to
  handle this:
  - Step 1: Update physics.
  - Step 2: Update visuals (what is rendered).
  - Step 3: Update logic (user input, etc.).
*/
void JumpingPlatformerScene::update(float delta) {
    // Step 1: Update the physics
    physicsEngine->update(delta);

    for (auto& sprite : sprites) {
        PhysicsBody2D* body =
          sprite->getPhysicsComponent()->getPhysicsBody().get();

        if (body->isDynamic) {
            // We must update all entities' positions based on the physics
            // simulation
            float alpha = physicsEngine->accumulator / physicsEngine->timeStep;

            // The physics and rendering loops are not synchronised. We can
            // solve this issue by extrapolating the position. This means we
            // predict what the future position is. In our case, the current
            // position is the one simulated by the physics engine, and the
            // future position is the render frame.
            Vector2 nextPosition = extrapolatePosition(body->getPosition(),
                                                       body->getVelocity(),
                                                       body->getAcceleration(),
                                                       alpha,
                                                       physicsEngine->timeStep);

            // Finally, set the position
            sprite->getTransformationComponent()->setPosition(
              Vector3(nextPosition, 0.0f));
        }

        // Lastly, we update the sprite's shader's view and projection matrices.
        camera->update(sprite->getShader());
    }

    // Step 2: Update the visuals
    BaseScene::update(delta);

    // Step 3: Update the logic
    glm::vec2 moveBy(0.0f);
    float speed = 20.0f;
    float jump = 40.0f;
    static bool isJumping = false;

    // Go right
    if (Input::isKeyPressed(FW_KEY_D) || Input::isKeyPressed(FW_KEY_RIGHT)) {
        playerSprite->addVelocity(speed * delta, 0.0f);
    }

    // Go left
    if (Input::isKeyPressed(FW_KEY_A) || Input::isKeyPressed(FW_KEY_LEFT)) {
        playerSprite->addVelocity(-speed * delta, 0.0f);
    }

    // Jump
    if (Input::isKeyPressed(FW_KEY_W)) { // && !isJumping) {
        playerSprite->addVelocity(0.0f, jump * delta);
        isJumping = true;
    }
}

void JumpingPlatformerScene::cleanUp() {}
