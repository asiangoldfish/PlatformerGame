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

public:
    Vector2 position;
    Vector2 velocity;
    bool isDynamic = false;
};
