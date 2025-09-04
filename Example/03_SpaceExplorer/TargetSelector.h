#pragma once

#include "BaseScene.h"
#include "Camera/Camera.h"

#include <glm/glm.hpp>

class TargetSelector : public FW::SceneNode {
public:
    TargetSelector();
    virtual ~TargetSelector() = default;

    virtual void update(float delta);

    void createTarget(glm::vec2 position);

public:
    FW::ref<FW::Camera> camera;
};
