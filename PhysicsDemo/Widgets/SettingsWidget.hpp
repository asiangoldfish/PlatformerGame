#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "ImGuiWidgetState.h"

namespace Editor {
    void exampleSettings();
    void windowSettings();

    void drawEditorPreferencesMenu(ImGuiWidgetState* state) {
ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
        
        if (ImGui::Begin("Example: Simple layout", &state->editorPreferences, ImGuiWindowFlags_MenuBar)) {
        // IMGUI_DEMO_MARKER("Examples/Simple layout");
            if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Close", "Ctrl+W")) { state->editorPreferences = false; }
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Left
            static int selectedOption = 0;
        {
            ImGui::BeginChild("left pane", ImVec2(150, 0), ImGuiChildFlags_Borders | ImGuiChildFlags_ResizeX);

                if (ImGui::Selectable("Window", selectedOption == 0)) {
                    selectedOption = 0;
                }

                if (ImGui::Selectable("Placeholder 1", selectedOption == 1)) {
                    selectedOption = 1;
                }

                if (ImGui::Selectable("Placeholder 2", selectedOption == 2)) {
                    selectedOption = 2;
            }

            ImGui::EndChild();
        }
        ImGui::SameLine();

        // Right
        {
            ImGui::BeginGroup();
            ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                
                switch(selectedOption) {
                    case 0:
                        windowSettings();
                        break;
                }

                ImGui::EndChild();
                if (ImGui::Button("Revert")) {}
                ImGui::SameLine();
                if (ImGui::Button("Save")) {}
                ImGui::EndGroup();
            }
        }

        ImGui::End();
    }

    void exampleSettings() {
        // ImGui::Text("MyObject: %d", selectedOption);
        // ImGui::Separator();
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. ");
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Details"))
                {
                    ImGui::Text("ID: 0123456789");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
    }

    void windowSettings() {
        if (ImGui::BeginTable("##properties", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_ScrollY)) {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch, 2.0f); // Default twice larger
            
            /* Font size */
            ImGui::TableNextRow();
            ImGui::PushID("Fontsize");
            ImGui::TableNextColumn();
            ImGui::AlignTextToFramePadding();
            ImGui::TextUnformatted("Font size");
            ImGui::TableNextColumn();

            int v_min = 4, v_max = 64;
            static int fontSize = 16;
            int imguiType = ImGuiDataType_S32;
            ImGui::SetNextItemWidth(-FLT_MIN);
            ImGui::SliderInt("##Editor", &fontSize, v_min, v_max);

            ImGui::PopID();
        }

        ImGui::EndTable();
}

} // namespace Editor