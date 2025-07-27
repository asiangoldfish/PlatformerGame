#pragma once

#include <imgui.h>
#include "imgui_internal.h"
#include "Framework.h"
#include "Files.h"
#include "../ExitStatus.h"

#include <filesystem>

void reloadImGui() {
    // Assume that the datadir exists.
    auto imguipath = FW::getDataDir().value() / "imgui.ini";
    INFO(imguipath.string());

    ImGui::LoadIniSettingsFromDisk(imguipath.string().c_str());
}

bool overrideImGuiConfig(const std::string& src) {
    return FW::getDataDir()
      .and_then([&](const std::filesystem::path& datadir) {
          std::filesystem::path imguiConfig = datadir / "imgui.ini";

          std::ifstream ifs(src, std::ios::binary);
          if (!ifs) {
              WARN("Failed to open source file: " + src);
              return std::expected<void, std::string>(
                std::unexpected("read error"));
          }

          std::ofstream ofs(imguiConfig, std::ios::binary | std::ios::trunc);
          if (!ofs) {
              WARN("Failed to open destination file: " + imguiConfig.string());
              return std::expected<void, std::string>(
                std::unexpected("write error"));
          }

          ofs << ifs.rdbuf();
          reloadImGui();
          return std::expected<void, std::string>{};
      })
      .has_value();
}

void displayLayoutMenu(GLFWwindow* window) {
    // NB! Changing the layout WILL restart the application. This to reload
    // ImGui's internal state.

    // Prevent the menu from closing after selecting a layout
    if (ImGui::MenuItem("Default")) {
        overrideImGuiConfig(std::filesystem::path(TEMPLATES_DIR) /
                            "imgui/default.ini");
        INFO("Default layout applied");
        ExitStatus::set(true);
        glfwSetWindowShouldClose(window, 1);
    }

    if (ImGui::MenuItem("Viewport only")) {
        overrideImGuiConfig(std::filesystem::path(TEMPLATES_DIR) /
                            "imgui/viewport_only.ini");
        INFO("Viewport only layout applied");
        ExitStatus::set(true);
        glfwSetWindowShouldClose(window, 1);
    }
}