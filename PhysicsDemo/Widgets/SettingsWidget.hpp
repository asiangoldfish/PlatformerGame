#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "ImGuiWidgetState.h"

namespace Editor {

    void drawEditorPreferencesMenu(ImGuiWidgetState* state) {
        ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);
        ImGui::Begin("Preferences", &state->editorPreferences);

        ImGui::Text("Hello from popup!");
        ImGui::Button("This is a dummy button..");

        ImGui::End();
    }

} // namespace Editor