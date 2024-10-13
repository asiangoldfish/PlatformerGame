#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>

#include "CustomControls.h"

namespace Editor {
    /**
     * Draw the properties panel. It dynamically fetches the properties of
     * selected entities in the scene.
     * @param app The physics app
     */
    void propertiesPanel(GLFWwindow* window) {
        ImGui::Begin("Properties");

        // glm::vec3 testControllers{0,0,0};

        // drawVec3Control("Test Controls,", testControllers);
            
        ImGui::End();
    }
} // namespace Editor