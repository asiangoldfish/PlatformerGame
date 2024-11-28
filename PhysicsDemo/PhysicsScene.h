#pragma once

#include "pch.h"
#include "Framework.h"

class PhysicsScene : public FW::Scene {
public:
    PhysicsScene();
    virtual ~PhysicsScene();
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

private:
    FW::Entity* drawableEntity;
};