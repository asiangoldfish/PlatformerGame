#pragma once

#include "Box.h"
#include "Camera/Camera.h"
#include "Entity.h"

/**
 * Container class for UI elements.
 *
 * To add an element, append it to GameUI::uiRoot's list of children. By calling
 * GameUi::draw() in the game scene, all elements will automatically be updated
 * and drawn.
 */
class GameUI {
public:
    GameUI();
    virtual ~GameUI() = default;

    /**
     * Draw all UIs. Call this function to spawn UIs on the screen.
     */
    void draw(float delta);

public:
    FW::UI::Box testBox;
    FW::ref<FW::Camera> camera;

    /**
     * The root of UI elements. The Sc
     */
    FW::ref<FW::Entity> uiRoot;
};