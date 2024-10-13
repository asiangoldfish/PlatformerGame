#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include <GLFW/glfw3.h>

namespace Editor {
    /**
     * Draw the main menu bar at the top of the window.
     * @param app Physics application.
     */
    void drawMenuBar(GLFWwindow* window) {
        ImGuiWindowFlags window_flags = 0;

        if (ImGui::BeginMainMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                ImGui::MenuItem("Files", NULL, false, false);
                if (ImGui::MenuItem("New")) {
                }
                if (ImGui::MenuItem("Open", "Ctrl+O")) {
                }
                if (ImGui::BeginMenu("Open Recent")) {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    if (ImGui::BeginMenu("More..")) {
                        ImGui::MenuItem("Hello");
                        ImGui::MenuItem("Sailor");
                        if (ImGui::BeginMenu("Recurse..")) {
                            //                            ShowExampleMenuFile();
                            ImGui::EndMenu();
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {
                }
                if (ImGui::MenuItem("Save As..")) {
                }

                ImGui::Separator();
                // ImGui::MenuItem("Settings", NULL, false, false);
                if (ImGui::BeginMenu("Preferences")) {
                    ImGui::MenuItem("General");
                    ImGui::MenuItem("Keyboard Shortcuts");
                    ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup,
                                        true);
                    if (ImGui::BeginMenu("Themes")) {
                        ImGui::MenuItem("Dark");
                        ImGui::MenuItem("Light");
                        ImGui::MenuItem("More...");
                        ImGui::EndMenu();
                    }
                    ImGui::PopItemFlag();
                    ImGui::EndMenu();
                }

                // Quit
                ImGui::Separator();
                ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup,
                                    true);
                if (ImGui::MenuItem("Quit Application", "Escape")) {
                    glfwSetWindowShouldClose(window, 1);
                }
                ImGui::PopItemFlag();

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit")) {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {
                }
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {
                } // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {
                }
                if (ImGui::MenuItem("Copy", "CTRL+C")) {
                }
                if (ImGui::MenuItem("Paste", "CTRL+V")) {
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    /**
     * Draw the properties panel. It dynamically fetches the properties of
     * selected entities in the scene.
     * @param app The physics app
     */
    void propertiesPanel(GLFWwindow* window) {
        if (ImGui::Begin("Properties")) {
            ImGui::End();
        }
    }
} // namespace Editor