#pragma once

#include <glm/glm.hpp>
#include "Framework.h"

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

    /** Draw all widgets. */
    void drawWidgets();

    /** Draw the 3d viewport */
    glm::vec2 drawViewport(uint32_t framebufferID);

    /** Draw a tree hierarchy with scenes and its entities */
    void drawSceneTree(FW::ref<FW::Scene> scene);

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

private:
    GLFWwindow* window;

};