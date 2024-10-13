/**
 * Useful controls for changing properties.
 *
 * This file contains controls that helps changing attributes or properties in
 * ImGui. For example, to change a glm::vec3 in runtime, you can draw a control
 * in a ImGui window using the following:
 *
 * Editor::drawVec3Control(...)
 *
 * @note This file is large. It is adviced to use an outliner, grep, find or
 *       something to search the file. Do also use the Doxygen documentation.
 *       To generate this, please review the run.sh script.
 *
 * @author Khai Duong
 * @date 13th of October 2024
 * @file CustomControls.h
 */

#pragma once

// ImGui libraries
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

// OpenGL libraries
#include <glm/glm.hpp>

// Standard libraries
#include <string.h>

namespace Editor {

    /**
     * Draw a vec3 control with label on the left-hand side
     *
     * @param label Label to display
     * @param values Editable values
     * @param resetValue Default value
     * @param columnWidth Width of the column between label and values
     */
    static void drawVec3Control(const std::string& label,
                                glm::vec3& values,
                                float resetValue = 0.0f,
                                float columnWidth = 100.0f) {
        // Add ID to distinguish between multiple ImGui components that invoke
        // this function
        ImGui::PushID(label.c_str());

        ImGui::Columns(2);

        ImGui::SetColumnWidth(0, columnWidth);
        ImGui::Text("%s", label.c_str()); // Label to display
        ImGui::NextColumn();

        ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });

        const float lineHeight =
          GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        const ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

        // --------------
        // Draw the ImGui elements...
        // --------------
        // X
        // TODO: Create UI library to store these color functions
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f });
        if (ImGui::Button("X", buttonSize)) {
            values.x = resetValue;
        }

        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##X", &values.x, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Y
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f });
        if (ImGui::Button("Y", buttonSize)) {
            values.y = resetValue;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Y", &values.y, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Z
        ImGui::PushStyleColor(ImGuiCol_Button,
                              ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                              ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f });
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                              ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f });
        if (ImGui::Button("Z", buttonSize)) {
            values.z = resetValue;
        }
        ImGui::PopStyleColor(3);

        ImGui::SameLine();
        ImGui::DragFloat("##Z", &values.z, 0.1f, 0.0f, 0.0f, "%.2f");
        ImGui::PopItemWidth();
        ImGui::SameLine();

        // Reset all values
        if (ImGui::Button("Reset", ImVec2{ lineHeight + 25.0f, lineHeight })) {
            values.x = resetValue;
            values.y = resetValue;
            values.z = resetValue;
        }

        ImGui::Columns(1);
        ImGui::PopStyleVar(1);
        ImGui::PopID();
    }

} // namespace Editor
