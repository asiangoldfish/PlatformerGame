#include "PhysicsEngine.h"
#include "Log.h"

PhysicsEngine::PhysicsEngine() {
    INFO("PhysicsEngine instance created");
}

PhysicsEngine::~PhysicsEngine() {
    physicsBodies.clear();
}

// Algorithm is based on:
// https://www.toptal.com/game/video-game-physics-part-i-an-introduction-to-rigid-body-dynamics
void PhysicsEngine::update(float delta) {
    if (timeSinceLastIteration < 1.0f / static_cast<float>(stepSize)) {
        timeSinceLastIteration += delta;
        return;
    } else {
        timeSinceLastIteration = 0.0f;
    }

    // Step 1: Apply the forces
    glm::vec3 totalAcceleration{ 0.0f };

    // Apply gravitational force
    const float gravity = -9.81f * 1.0f / static_cast<float>(stepSize);
    for (auto body : physicsBodies) {
        if (body->isDynamic) {
            body->velocity.y += gravity;
        }
    }

    // for (auto& force : forces) {
    //     force->update();
    // }

    // Step 2: Update positions and velocity

    // Step 3: Detect collisions

    // Step 4: Solve constraints
}

void PhysicsEngine::addPhysicsBody(ref<PhysicsBody2D> body) {
    physicsBodies.push_back(body);
}
