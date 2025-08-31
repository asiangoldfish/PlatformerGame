#pragma once

#include "ProgressBar.h"
#include "Camera/Camera.h"
#include "UIRoot.h"

/**
 * Container class for UI elements.
 *
 * To add an element, append it to GameUI::uiRoot's list of children. By calling
 * GameUi::draw() in the game scene, all elements will automatically be updated
 * and drawn.
 */
class GameUI {
public:
    GameUI() = default;
    virtual ~GameUI() = default;

    void init();

    /**
     * Draw all UIs. Call this function to spawn UIs on the screen.
     */
    void draw(float delta);

public:
    /**
     * Add a new callable handler if it already isn't registered.
     *
     * @param id string representation of the function. Must be unique for each
     * handler.
     * @param func function to call.
     */
    void addHandler(const std::string& id, std::function<void()> func);

    /**
     * Remove a callable handler.
     *
     * @param id string representation of the function. Must be unique for each
     * handler.
     */
    void removeHandler(const std::string& id);

private:
    void invokeAllHandlers();

public:
    FW::ref<FW::UI::ProgressBar> healthBar;
    FW::ref<FW::Camera> camera;

    /**
     * The root of UI elements. The Sc
     */
    FW::ref<FW::UI::UIRoot> uiRoot;

private:
    std::vector<std::pair<std::string, std::function<void()>>> handlers;
};