#pragma once

#include <glm/glm.hpp>
#include "Framework.h"
#include "../Selections.h"

/**
 * Container of states for opened or closed widgets.
 */
struct WidgetStateContainer {
    bool editorPreferences = false;
    bool showDemo = false;

    /// @brief If true, then update the font size before the next new frame.
    bool shouldUpdateFontSize = false;
};

/**
 * We need to store the mouse state, because our GLFW application does not
 * register mouse events by default.
 */
struct MouseState {
    bool isLeftButtonDown = false;
    bool isRightButtonDown = false;
    bool isViewportFocused = false;
    bool isViewportHovered = false;
    glm::vec2 mousePosition = glm::vec2(0,0);
};

/**
 * DearImGUI has a weird system for popups, and one of the requirements is the
 * popups must be called within the same ID stack and must be implemented
 * correctly in menus. As a work around, we make have a bool for each
 * specific type of popup that users can call. AppWidget will then open
 * the specific popups if the corresponding bool is true.
 */
struct Popups {
    bool saveCustomLayout = false;
    bool deleteCustomLayout = false;
};

/**
 * The AppWidget class provides an interface to interact with widgets.
 * 
 * A widget is an additional window created by DearImGUI to provide a feature
 * rich application development environment.
 */
class AppWidget {
public:
    AppWidget() = default;
    virtual ~AppWidget();

    /** Initialise widgets */
    void init(GLFWwindow* window);

    /** Prepare ImGui for drawing. Must be called before drawWidgets() */
    void beginDraw();

    /** End ImGui drawing */
    void endDraw();

    /** Draw popup windows */
    void drawPopups();

    /** Draw all widgets. */
    void drawWidgets();

    /** Draw the 3d viewport */
    glm::vec2 drawViewport(uint32_t framebufferID);

    /** Draw a tree hierarchy with scenes and its entities */
    void drawSceneTree(FW::ref<FW::BaseScene> scene);

    /** Set the base font size across all widgets */
    void setFontSize(float size);
private:
    /** Enable ImGui docking */
    void docking();

    /**
     * Draw the main menu bar at the top of the window.
     */
    void drawMenuBar();

    /**
     * Draw the properties panel. It dynamically fetches the properties of
     * selected entities in the scene.
     */
    void propertiesPanel();

    /**
     * Draw the preference menu. Users can adjust engine preferences here.
     */
    void drawEditorPreferencesMenu();

    void windowSettings();

public:
    WidgetStateContainer widgetStates;
    int fontSize = 8;
    FW::ref<FW::JSONParser> editorConfig;
    MouseState mouseState;
    Popups popups;

    FW::ref<SelectedNode> selectedNode;

private:
    GLFWwindow* window;

};