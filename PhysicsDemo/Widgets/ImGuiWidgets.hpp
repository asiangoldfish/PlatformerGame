#pragma once

// ImGui internal libraries
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

// Standard libraries
#include <fstream>
#include <string>

// Graphics libraries
#include <GLFW/glfw3.h>

// Custom ImGui widgets
#include "MenuBarWidget.hpp"
#include "SettingsWidget.hpp"

namespace Editor {
    /**
     * This struct provides a method to store persistent state across widgets.
     */
    struct ImGuiWidgetsState {
        bool isSettingsVisible = false;
    };

    void ImGuiDocking();

    /**
     * This function provides a quick API to draw custom ImGui widgets.
     */
    void drawImguiWidgets(GLFWwindow* window) {
        drawMenuBar(window);
        propertiesPanel(window);
    }

    /**
     * Initialise Dear ImGui.
     *
     * Required ImGui parameters are initialised. They are project specific.
     * Users can edit this function for specific needs. Some function calls are
     * required; however, and omitting these can lead to crashes.
     *
     * @param window The GL window to draw on.
     */
    void initEditorImgui(GLFWwindow* window, FW::ref<FW::JSONParser> editorConfig) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();

        // -----
        // Configure ImGui
        // -----
        // When clicking on a drag component, user can enter a value
        io.ConfigDragClickToInputText = true;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
        // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

        // Styling
        ImGui::StyleColorsDark();

        // When viewports are enabled we tweak WindowRounding/WindowBg so
        // platform windows can look identical to regular ones.
        ImGuiStyle& style = ImGui::GetStyle();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            style.WindowRounding = 0.0f;
            // style.Colors[ImGuiCol_WindowBg].w = 1.0f;
        }

        // Scale
        // ImGuiStyle::ScaleAllSizes(2.0);
        style.ScaleAllSizes(1.0);

        // Source: https://fonts.google.com/specimen/Open+Sans
        std::string fontPath =
          RESOURCES_DIR +
          std::string("fonts/Open_Sans/static/OpenSans-Regular.ttf");
        io.Fonts->AddFontFromFileTTF(
          fontPath.c_str(),
          editorConfig->get()["ui"]["fontSize"]); //, nullptr, nullptr);
        // io.Fonts->GetTexDataAsRGBA32();

        // Setup ImGui backends for OpenGL
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 430");
    }

    /**
     * Must be called at the beginning of each frame whenever ImGui is drawing.
     * 
     * @param window GLFW window which the imgui widgets are rendered to.
     */
    void beginImGuiDraw(GLFWwindow* window) {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        Editor::ImGuiDocking();

        // Uncomment this to see the demo window.
        // ImGui::ShowDemoWindow();

        drawImguiWidgets(window);
    }

    /**
     * Must be called at the end of each frame after all other ImGui draw calls.
     */
    void endImGuiDraw() {
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
     * Draw the OpenGL viewport
     * @param app The physics app
     * @param framebufferId the id of the buffer that the viewport is drawn on.
     * @return the viewport's new size
     */
    glm::vec2 drawViewport(uint32_t framebufferId) {
        if (ImGui::Begin("Viewport")) {
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();
            // TODO Ugly code. Reformat this.
            ImGui::Image(
              (void*)(uint64_t)framebufferId,
              viewportSize,
              ImVec2{ 0, 1 },
              ImVec2{ 1, 0 });
            ImGui::End();

            return glm::vec2{ viewportSize.x, viewportSize.y };
        }

        return glm::vec2{};
    }

    /**
     * Create a dockspace.
     *
     * @details This function should be called at the beginning of each frame,
     * after calling <u>beginImGuiDraw()</u>
     */
    void ImGuiDocking() {
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
