#include "AppWidget.h"

// ImGui internal libraries
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_internal.h>

void sceneTreeEntry(FW::ref<FW::Entity> entity);

void AppWidget::beginDraw() {
    // Because some things must be set or defined before NewFrame is called, we
    // do them here.
    if (widgetStates.shouldUpdateFontSize) {
        widgetStates.shouldUpdateFontSize = false;
        setFontSize(fontSize);
        editorConfig->get()["ui"]["fontSize"] = fontSize;
        editorConfig->write(true);
    }

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    docking();
}

void AppWidget::endDraw() {
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

void AppWidget::drawWidgets() {
    drawMenuBar();
    propertiesPanel();

    if (widgetStates.editorPreferences) {
        drawEditorPreferencesMenu();
    }

    // Uncomment this to see the demo window.
    if (widgetStates.showDemo) {
        ImGui::ShowDemoWindow();
    }
}

glm::vec2 AppWidget::drawViewport(uint32_t framebufferID) {
    if (ImGui::Begin("Viewport")) {
        ImVec2 viewportSize = ImGui::GetContentRegionAvail();
        // TODO Ugly code. Reformat this.
        ImGui::Image(
            (unsigned long long)framebufferID,
            viewportSize,
            ImVec2{ 0, 1 },
            ImVec2{ 1, 0 });

        mouseState.isViewportHovered = ImGui::IsItemHovered();
        mouseState.isViewportFocused = ImGui::IsItemFocused();
        mouseState.mousePosition = glm::vec2(
            ImGui::GetMousePos().x, ImGui::GetMousePos().y);
        mouseState.isLeftButtonDown = ImGui::IsMouseDown(ImGuiMouseButton_Left);
        mouseState.isRightButtonDown = ImGui::IsMouseDown(ImGuiMouseButton_Right);

        ImGui::End();

        return glm::vec2{ viewportSize.x, viewportSize.y };
    }

    ImGui::End();
    return glm::vec2{};
}

void AppWidget::drawSceneTree(FW::ref<FW::BaseScene> scene) {
    ImGui::Begin("SceneTree");
    sceneTreeEntry(scene->getRoot());
    ImGui::End();
}

void sceneTreeEntry(FW::ref<FW::Entity> entity) {
    ImGui::SetNextItemOpen(entity->getChildren().size(), ImGuiCond_Once);
    ImGui::PushID(entity->name.c_str());
    if (ImGui::TreeNode(entity->name.c_str()))
    {
        for (auto child: entity->getChildren()) {
            sceneTreeEntry(child);
        }

        ImGui::TreePop();
    }
    ImGui::PopID();
}

void AppWidget::drawMenuBar() {
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
            // if (ImGui::BeginMenu("Preferences")) {
            //     if (ImGui::Button("Open Preferences")) {
            //         INFO("Hi");
            //         state.isSettingsVisible = true;
            //     }
            //     ImGui::MenuItem("Keyboard Shortcuts");
            //     ImGui::PushItemFlag(ImGuiItemFlags_SelectableDontClosePopup,
            //                         true);
            //     if (ImGui::BeginMenu("Themes")) {
            //         ImGui::MenuItem("Dark");
            //         ImGui::MenuItem("Light");
            //         ImGui::MenuItem("More...");
            //         ImGui::EndMenu();
            //     }
            //     ImGui::PopItemFlag();
            //     ImGui::EndMenu();
            // }
            ImGui::MenuItem("Preferences", NULL, &widgetStates.editorPreferences);

            // Demo
            ImGui::Separator();
            ImGui::MenuItem("ImGui Demo", NULL, &widgetStates.showDemo);


            // Quit
            ImGui::Separator();
            ImGui::PushItemFlag(ImGuiItemFlags_AutoClosePopups,
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

void AppWidget::propertiesPanel() {
    ImGui::Begin("Properties");

    // glm::vec3 testControllers{0,0,0};

    // drawVec3Control("Test Controls,", testControllers);
        
    ImGui::End();
}

void AppWidget::drawEditorPreferencesMenu() {
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
        
        if (ImGui::Begin("Example: Simple layout", &widgetStates.editorPreferences, ImGuiWindowFlags_MenuBar)) {
            // IMGUI_DEMO_MARKER("Examples/Simple layout");
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File"))
                {
                    if (ImGui::MenuItem("Close", "Ctrl+W")) { widgetStates.editorPreferences = false; }
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
                if (ImGui::Button("Save")) {
                    widgetStates.shouldUpdateFontSize = true;
                }
                ImGui::EndGroup();
            }
        }

        ImGui::End();
}

void AppWidget::windowSettings() {
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
        int imguiType = ImGuiDataType_S32;
        ImGui::SetNextItemWidth(-FLT_MIN);
        ImGui::SliderInt("##fontsize", &fontSize, v_min, v_max);

        ImGui::PopID();
    }

    ImGui::EndTable();
}

void AppWidget::init(GLFWwindow* window) {
    this->window = window;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();

    // Because imgui stores the imgui.ini config in CWD by default, we want to
    // store it in the user's local storage which is OS-dependent.
    // TODO: Add support in the Engine to retrieve the user's local storage.
    std::string userDataPath;
    char* appdata = std::getenv("APPDATA");
    if (appdata) {
        userDataPath = std::string(appdata) + "\\PhysicsDemo\\imgui.ini";
        io.IniFilename = NULL;
        ImGui::LoadIniSettingsFromDisk(userDataPath.c_str());
    }

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

    // Source: https://fonts.google.com/specimen/Open+Sans
    std::string fontPath =
        RESOURCES_DIR +
        std::string("fonts/Open_Sans/static/OpenSans-Regular.ttf");
    io.Fonts->AddFontFromFileTTF(
        fontPath.c_str(),
        16.0); //, nullptr, nullptr);

    setFontSize(fontSize);

    // io.Fonts->GetTexDataAsRGBA32();

    // Setup ImGui backends for OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void AppWidget::docking() {
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
}

void AppWidget::setFontSize(float size) {
    fontSize = size;
    ImGuiIO& io = ImGui::GetIO();
    
    io.Fonts->Clear();
    
    std::string fontPath =
        RESOURCES_DIR +
        std::string("fonts/Open_Sans/static/OpenSans-Regular.ttf");

    ImFont* newFont = io.Fonts->AddFontFromFileTTF(fontPath.c_str(), size);
    if (!newFont) {
        std::cerr << "Failed to load font from: " << fontPath << std::endl;
        return;
    }

    // Rebuild the font atlas
    unsigned char* texData;
    int texWidth, texHeight;
    io.Fonts->GetTexDataAsRGBA32(&texData, &texWidth, &texHeight);

    // Upload the font texture to the GPU (example using OpenGL)
    uint32_t fontTexture;
    glGenTextures(1, &fontTexture);
    glBindTexture(GL_TEXTURE_2D, fontTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, texData);

    // Set texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Inform ImGui of the texture ID
    io.Fonts->SetTexID((ImTextureID)(uint64_t)fontTexture);

    // Set the new font as the default font
    io.FontDefault = newFont;
}

AppWidget::~AppWidget() {
    // We handle managing imgui.ini ourselves, because we have a custom
    // location. Therefore we should save the config file before we shut down.
    std::string userDataPath;
    char* appdata = std::getenv("APPDATA");
    if (appdata) {
        ImGuiIO& io = ImGui::GetIO();
        userDataPath = std::string(appdata) + "\\PhysicsDemo\\imgui.ini";
        io.IniFilename = NULL;
        ImGui::SaveIniSettingsToDisk(userDataPath.c_str());
    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}