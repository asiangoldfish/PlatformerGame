#pragma once

#include "BaseScene.h"
#include "Camera/Camera.h"

#include <glm/glm.hpp>

/**
 *
 */
enum TargetSelectionState { INACTIVE = 0, HOVERED, ACTIVE };

/**
 * A ring that surrounds a target, indicating that it is selected.
 */
class TargetSelector : public FW::SceneNode {
public:
    TargetSelector() = default;
    TargetSelector(FW::ref<FW::Camera> camera);
    virtual ~TargetSelector() = default;

    virtual void update(float delta);

    void setSelectionState(TargetSelectionState type);
    TargetSelectionState getSelectionState() { return selectionState; }

private:
    void setColor(TargetSelectionState type);

private:
    TargetSelectionState selectionState = TargetSelectionState::INACTIVE;
    glm::vec4 color{ 1.0f };
};
