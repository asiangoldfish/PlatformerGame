#pragma once

namespace Editor {

    /**
     * This struct provides a method to store persistent state across widgets.
     */
    struct ImGuiWidgetState {
        bool editorPreferences = false;
        bool showDemo = false;
    };

} // namespace Editor