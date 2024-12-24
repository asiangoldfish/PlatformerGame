#pragma once

#include "pch.h"
#include "Types.h"

/**
 * The PhysicsBody is an entity is the "physics world". It interacts with
 * forces and other bodies.
 */
class PhysicsBody2D {
public:
    PhysicsBody2D() = default;
    virtual ~PhysicsBody2D() = default;

    /**
     * Adding velocity doesn't directly impact the velocity on the next
     * rendering frame. Because velocity is tied to physics, we request the
     * physics engine to consume our added velocity.
     *
     * For example: if our the physics frequency is 30 MHz and the rendering
     * frequency (which the user input is tied to) is 60 MHz, then depending on
     * our configuration the user can request velocity change twice before the
     * physics engine consumes the requests.
     */
    void addVelocity(Vector2 vel) { requestedVelocity += vel; }

    /**
     * Get this body's velocity.
     *
     * This is the value computed by the Physics Engine. If you want to retrieve
     * the value requested before the next step, see
     * PhysicsBody2D::getRequestedVelocity().
     */
    Vector2 getVelocity() { return velocity; }

    /**
     * In edge cases where the requested velocity is required, this method may
     * be used. It returns a copy of the velocity. To add velocity, see
     * PhysicsEngine::addVelocity().
     */
    Vector2 getRequestedVelocity() { return requestedVelocity; }

    /**
     * Get the body's acceleration vector.
     *
     * This value is computed by the Physics Engine.
     */
    Vector2 getAcceleration() { return acceleration; }

    Vector2 getPosition() { return position; }
    void setPosition(Vector2 position) { this->position = position; }

public:
    /**
     * isDynamic determines whether the body is affected by collisions or
     * forces. If it is true, then it is considered a rigid body. Otherwise it
     * is considered a static body.
     */
    bool isDynamic = false;

private:
    friend class PhysicsEngine;
    /// The current position
    Vector2 position;

    /// Current velocity. This is the result in
    Vector2 velocity{ 0.0f };
    Vector2 previousVelocity{ 0.0f };

    /**
     * This value represents the velocity change accumulated by external forces.
     *
     * Whenever the physics body receives is impacted by a force outside of the
     * Physics Engine's domain (like user input), the Physics Engine expects
     * that velocity changes is requested.
     *
     * The Physics Engine will reset this value to { 0.0 } after it is consumed.
     * This means you can for example check if a user input has been processed
     * yet.
     */
    Vector2 requestedVelocity{ 0.0f };

    Vector2 acceleration;
};
