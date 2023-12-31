#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

#include "PhysicsApp.h"

namespace Game {
    void ImGuiDocking();

    /**
     * Must be called at the beginning of each frame whenever ImGui is drawing.
     */
    void beginImGuiDraw()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        // ImGui::ShowDemoWindow();
    }

    /**
     * Must be called at the end of each frame after all other ImGui draw calls.
     */
    void endImGuiDraw()
    {
        // Render ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backup_current_context = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backup_current_context);
        }
    }

    /**
     * Draw the main menu bar at the top of the window.
     * @param app Physics application.
     */
    void drawMenuBar(PhysicsApp& app)
    {
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
                ImGui::MenuItem("Settings", NULL, false, false);
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
                    glfwSetWindowShouldClose(app.getWindow(), 1);
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
    void propertiesPanel(PhysicsApp& app)
    {
        if (ImGui::Begin("Properties")) {
            ImGui::End();
        }
    }

    /**
     * Draw the OpenGL viewport
     * @param app The physics app
     */
    void viewport(PhysicsApp& app)
    {
        if (ImGui::Begin("Viewport")) {
            ImGui::Image((void*)app.getViewportFramebuffer()->getTexture(), ImVec2(720, 480),
                         ImVec2{ 0, 1 },
                         ImVec2{ 1, 0 });
            ImGui::End();
        }
    }

    /**
     * Create a dockspace.
     *
     * @details This function should be called at the beginning of each frame,
     * after calling <u>beginImGuiDraw()</u>
     */
    void ImGuiDocking()
    {
        // ------
        // BEGIN Dockspace
        // ------

        // Set to true to enable dockspace
        static bool enableDockspace = true;
        // ImGui Dockspace
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static bool dockspaceOpen = true;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the
        // parent window not dockable into, because it would be confusing to
        // have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen) {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar |
                            ImGuiWindowFlags_NoCollapse |
                            ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus |
                            ImGuiWindowFlags_NoNavFocus;
        } else {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace()
        // will render our background and handle the pass-thru hole, so we
        // ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false
        // (aka window is collapsed). This is because we want to keep our
        // DockSpace() active. If a DockSpace() is inactive, all active
        // windows docked into it will lose their parent and become
        // undocked. We cannot preserve the docking relationship between an
        // active window and an inactive docking, otherwise any change of
        // dockspace/settings would lead to windows being stuck in limbo and
        // never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding,
                                ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // Submit the DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }
        ImGui::End();

        // ------
        // END Dockspace
        // ------
    }
}
