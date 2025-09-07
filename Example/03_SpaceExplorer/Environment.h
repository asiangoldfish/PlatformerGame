#pragma once

#include "BaseScene.h"
#include "Serializable.h"
#include "Camera/Camera.h"

class Asteroid : public FW::SceneNode, public FW::Serializable {
public:
    Asteroid() = default;
    Asteroid(FW::ref<FW::Camera> camera);
    virtual ~Asteroid() = default;

    virtual void serialize() override {}
    virtual void deserialize() override {}

    virtual void update(float delta) override;
};