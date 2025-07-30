#pragma once

#include <imgui.h>

/**
 * Helper class to apply universal interface across the application.
 */
class WidgetStyle {
public:
    WidgetStyle() = default;
    virtual ~WidgetStyle() = default;

    ImVec2 getButtonSize() {
        return ImVec2{ImGui::GetFontSize() * 7.0f, 0.0f};
    }
private:
};
