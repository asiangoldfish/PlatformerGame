#pragma once

class PhysicsBody {
public:
    PhysicsBody() = default;
    virtual ~PhysicsBody() = default;

    virtual void update(float delta) = 0;
};

class RigidBody : public PhysicsBody {
public:
    RigidBody() = default;
    virtual ~RigidBody() = default;

    virtual void update(float delta);
};
