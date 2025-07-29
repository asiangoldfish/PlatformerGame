#pragma once

#include <imgui.h>
#include "imgui_internal.h"
#include "Framework.h"
#include "Files.h"
#include "../ExitStatus.h"
#include "Log.h"

#include <filesystem>
#include <cctype>

void reloadImGui() {
    // Assume that the datadir exists.
    auto imguipath = FW::getDataDir().value() / "imgui.ini";

    ImGui::LoadIniSettingsFromDisk(imguipath.string().c_str());
}

/**
 * Copy template configuration to DearImGUI configuration
 */
bool overrideImGuiConfig(const std::string& name) {
    std::filesystem::path datadir = FW::getDataDir().value();
    std::filesystem::path templatesdir = datadir / "templates";
    std::filesystem::path imguiConfig = datadir / "imgui.ini";

    std::ifstream ifs(templatesdir / name, std::ios::binary);
    if (!ifs) {
        WARN("Failed to open source file: " +
             std::filesystem::path(templatesdir / name).string());
        return false;
    }

    std::ofstream ofs(imguiConfig, std::ios::binary | std::ios::trunc);
    if (!ofs) {
        WARN("Failed to open destination file: " + imguiConfig.string());
        return false;
    }

    ofs << ifs.rdbuf();
    reloadImGui();
    return true;
}

/**
 * Save the current DearImGUI layout as a new custom template.
 */
bool saveLayoutAs(std::string newName) {
    std::filesystem::path datadir = FW::getDataDir().value();
    std::filesystem::path customTemplatesDir = datadir / "templates" / "custom";
    std::filesystem::path imguiConfig = datadir / "imgui.ini";

    std::string newConfigName;

    for (char& c : newName) {
        // The name is assumed PascalCase. We add underscore in the file name
        // to signify there is a space there. Makes displaying the template
        // name easier.
        if (!newConfigName.empty() and isupper(c)) {
            newConfigName += '_';
        }

        newConfigName += c;
    }

    newConfigName += ".ini";
    std::filesystem::path newCustomConfigPath =
      customTemplatesDir / newConfigName;

    std::ifstream ifs(imguiConfig, std::ios::binary);
    if (!ifs) {
        WARN("Failed to open source file: " + imguiConfig.string());
        return false;
    }

    std::ofstream ofs(newCustomConfigPath, std::ios::binary | std::ios::trunc);
    if (!ofs) {
        WARN("Failed to open destination file: " +
             newCustomConfigPath.string());
        return false;
    }

    ImGui::SaveIniSettingsToDisk(imguiConfig.c_str());

    ofs << ifs.rdbuf();

    ifs.close();
    ofs.close();

    return true;
}

void displayLayoutMenu(GLFWwindow* window, Popups& popups) {
    if (ImGui::MenuItem("Save Layout")) {
        std::filesystem::path path = FW::getDataDir().value() / "imgui.ini";
        ImGui::SaveIniSettingsToDisk(path.c_str());
    }
    if (ImGui::MenuItem("Save Layout As")) {
        popups.saveCustomLayout = true;
    }
    if (ImGui::MenuItem("Delete Layout...")) {
        popups.deleteCustomLayout = true;
    }
    ImGui::Separator();

    // NB! Changing the layout WILL restart the application. This to reload
    // ImGui's internal state.
    ImGui::Text("TEMPLATE LAYOUTS");
    ImGui::Separator();
    // Template layout: Prevent the menu from closing after selecting a layout
    if (ImGui::MenuItem("Default")) {
        overrideImGuiConfig("default.ini");
        INFO("Default layout applied");
        ExitStatus::set(true);
        glfwSetWindowShouldClose(window, 1);
    }

    if (ImGui::MenuItem("Viewport Only")) {
        overrideImGuiConfig("viewport_only.ini");
        INFO("Viewport only layout applied");
        ExitStatus::set(true);
        glfwSetWindowShouldClose(window, 1);
    }
    ImGui::Separator();

    // Custom layouts
    // We assume that it exists. This may be dangerous and cause runtime error
    // the user or some processes delete it.
    ImGui::Text("CUSTOM LAYOUTS");
    ImGui::Separator();
    std::string customLayoutsPath =
      FW::getDataDir().value() / "templates" / "custom";
    for (const auto& entry :
         std::filesystem::directory_iterator(customLayoutsPath)) {

        // Construct the title based on the filename.
        std::string filenameNoExtension =
          entry.path().filename().replace_extension().string();

        std::string pascalFilename;

        for (char& c : filenameNoExtension) {
            // Use underscore character as space. We prevent this from happening
            // if the new filename is empty, so we don't get a weird space at
            // the beginning
            if (c == '_' && pascalFilename.size()) {
                pascalFilename.append(" ");
            } else {
                pascalFilename += c;
            }
        }

        if (ImGui::MenuItem(pascalFilename.c_str())) {
            overrideImGuiConfig("custom/" + filenameNoExtension + ".ini");
            INFO(pascalFilename + " layout applied");
            ExitStatus::set(true);
            glfwSetWindowShouldClose(window, 1);
        }
    }
}