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
    accumulator += delta;

    // If there is enough time to perform multiple steps, the while loop allows
    // us to do so.
    while (accumulator >= timeStep) {
        accumulator -= timeStep;

        // Step 1: Apply the forces
        glm::vec3 totalAcceleration{ 0.0f };

        // Apply gravitational force
        const float gravity = -9.81f * timeStep;
        for (auto body : physicsBodies) {
            if (body->isDynamic) {
                body->previousVelocity = body->velocity;

                // Consume requested velocity
                body->velocity += body->requestedVelocity;
                body->requestedVelocity.x = 0.0f;
                body->requestedVelocity.y = 0.0f;

                // Add gravity
                body->velocity.y += gravity;
                body->acceleration =
                  (body->velocity - body->previousVelocity) / timeStep;
            }
        }

        // Step 2: Update positions and velocity
        for (auto body : physicsBodies) {
            if (body->isDynamic) {
                body->position += body->velocity;
            }
        }

        // Step 3: Detect collisions

        // Step 4: Solve constraints
    }
}

void PhysicsEngine::addPhysicsBody(ref<PhysicsBody2D> body) {
    physicsBodies.push_back(body);
}
