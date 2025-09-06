#include "Debugging.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <GLFW/glfw3.h>

Debugging::~Debugging() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void Debugging::init(GLFWwindow* window) {
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

    // Source: https://fonts.google.com/specimen/Open+Sans
    std::string fontPath =
      RESOURCES_DIR +
      std::string("fonts/Open_Sans/static/OpenSans-Regular.ttf");
    io.Fonts->AddFontFromFileTTF(fontPath.c_str(),
                                 16.0); //, nullptr, nullptr);

    // Setup ImGui backends for OpenGL
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
}

void Debugging::draw(float delta) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // ====== BEGIN DRAWING STUFF =============
    // ImGui::ShowDemoWindow();
    drawShipStats();
    drawCurrencies();
    // ====== END DRAWING STUFF ===============

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

void Debugging::drawShipStats() {
    ImGui::Begin("Ship Stats");

    // Player ship
    // =======================
    ImGui::Text(playerShip->entity->name.c_str());
    ImGui::Text("\tHP:");
    ImGui::SameLine();
    ImGui::Text("%.2f", playerShip->vitalStats.health);


    ImGui::Text("Position:");
    ImGui::SameLine();
    glm::vec2 playerPos = playerShip->getPosition();
    ImGui::Text("(%.0f:%.0f)", playerPos.x, playerPos.y);

    ImGui::Separator();

    // EnemyShip
    // =======================
    ImGui::Text(enemyShip->entity->name.c_str());
    ImGui::Text("\tHp:");
    ImGui::SameLine();
    ImGui::Text("%.2f", enemyShip->vitalStats.health);

    ImGui::Text("Position:");
    ImGui::SameLine();
    glm::vec2 enemyPos = enemyShip->getPosition();
    ImGui::Text("(%.0f:%.0f)", enemyPos.x, enemyPos.y);

    // Case mode
    ImGui::Text("Chase mode:");
    ImGui::SameLine();
    ImGui::Text(enemyShip->chaseModeToStr().c_str());

    if (enemyShip->chaseMode == EnemyShip::AIChaseMode::PATROLCOOLDOWN) {
        ImGui::SameLine();
        ImGui::Text("(%.2fs)", enemyShip->getPatrollingCooldown());
    } else if (enemyShip->chaseMode == EnemyShip::AIChaseMode::PATROLLING) {
        ImGui::Text("Next position:");
        ImGui::SameLine();
        glm::vec2 targetPosition = enemyShip->getPatrollingTargetPosition();
        ImGui::Text("(%.0f:%.0f)", targetPosition.x, targetPosition.y);
    }

    ImGui::End();
}

void Debugging::drawCurrencies() {
    ImGui::Begin("Currencies");

    // Player ship
    // =======================
    ImGui::Text("\tCash:");
    ImGui::SameLine();
    ImGui::Text("%i", playerShip->currenciesStats.cash);

    ImGui::End();
}