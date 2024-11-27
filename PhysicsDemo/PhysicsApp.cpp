/**
 * Copyright 2023-2024 Khai Duong
 */

#include "PhysicsApp.h"

// Framework API
#include "Framework.h"

// App Components
#include "Cube.h"
#include "Floor.h"
#include "WorldGrid.h"

#include "Widgets/ImGuiWidgets.hpp"

bool PhysicsApp::init() {
    // ------
    // Configure application
    // ------
    if (!GLFWApplication::init()) {
        return false;
    }

    // ------
    // Configurations
    // ------
    configureDefaultEditorSettings();

    // TODO Move this to Framework as an engine feature.
    // Initialise the window mode if this is set in the configurations.
    std::string defaultWindowMode = editorConfig->get()["window"]["windowMode"];
    if (defaultWindowMode == "window") {
        changeWindowMode(FW::WindowMode::WINDOW);
    } else if (defaultWindowMode == "borderless") {
        changeWindowMode(FW::WindowMode::BORDERLESS);
    } else if (defaultWindowMode == "fullscreen") {
        changeWindowMode(FW::WindowMode::FULLSCREEN);
    } else {
        WARN("defaultWindowMode value '{0}' in PhysicsApp is invalid",
             defaultWindowMode);
    }

    // -----------
    // Textures and shaders
    // -----------
    shader = FW::createRef<FW::Shader>(
      RESOURCES_DIR + std::string("shaders/vertex.glsl"),
      RESOURCES_DIR + std::string("shaders/fragment.glsl"));

    worldGridShader = FW::createRef<FW::Shader>(
      RESOURCES_DIR + std::string("shaders/worldGridVertex.glsl"),
      RESOURCES_DIR + std::string("shaders/worldGridFrag.glsl"));

    // We only have one shader in the application, so we only bind it here.
    shader->bind();
    getShader()->setInt("u_material.diffuse", 0);
    getShader()->setInt("u_material.specular", 1);

    // -------------
    // Entities
    // -------------
    FW::TextureManager::loadTexture2D(
      "metal_plate_diff",
      RESOURCES_DIR +
        std::string("textures/polyhaven/metal_plate/metal_plate_diff_1k.jpg"));

    playerCube = FW::createRef<Cube>();
    playerCube->setScale({ 1.0f, 1.0f, 1.0f });
    playerCube->setPosition({ 0, playerCube->getScale().y / 2.0f, 0 });
    playerCube->getMaterial().getProperties().setDiffuseTextureID(
      "metal_plate_diff");
    playerCube->setMaterial(FW::MaterialPreset::CHROME);
    playerController = FW::createScope<FW::PlayerController>(playerCube);

    // ---------
    // Rendering
    // ---------
    // Configure camera
    cameraController =
      FW::createRef<FW::CameraController>(FW::CameraType::PERSPECTIVE);
    cameraController->getPerspectiveCamera()->setEnablePanning(true);
    cameraController->addRotation({ -90.0f, -45.0f });
    cameraController->setPosition({ 0.0f, 5.0f, 5.0f });
    cameraController->setSpectatorMode(true);

    // Screen
    RenderCommand::setClearColor(glm::vec3(0.5f, 0.5f, 0.5f));

    testCube = FW::createRef<Cube>();
    testCube->setPosition({ 0.0f, 0.0f, -3.0f });
    testCube->setScale(1.0f);

    // Grid
    int gridSize = 100;
    grid = FW::createScope<Floor>(gridSize);
    grid->setRotation({ 90.0f, 0.0f, 0.0f });
    grid->setPosition({ -gridSize / 2, 0, -gridSize / 2 });

    RenderCommand::setClearColor(glm::vec3{ 0.2f, 0.1f, 0.215f });
    shader->setVisualizeMode(RenderCommand::VisualizeMode::NORMAL);

    worldGrid = FW::createRef<WorldGrid>();

    // Physics
    emitter = FW::createScope<FW::Emitter>();
    emitter->setMaxLifetime(1.0f, 3.0f);
    float spread = 0.075f;
    emitter->setInitialVelocityX(-spread, spread);
    emitter->setInitialVelocityY(0.3f, 0.5f);
    emitter->setInitialVelocityZ(-spread, spread);
    emitter->setGravity(0.0098f);
    emitter->setMaxParticles(100);

    // Init Dear ImGui
    Editor::initEditorImgui(getWindow(), editorConfig);


    // Framebuffer for rendering GL on ImGui
    viewportFramebuffer = FW::createRef<FW::Framebuffer>();

    INFO("Client application successfully initialized");

    return true;
}

void PhysicsApp::run() {
    FW::Timer emitterTimer;
    emitterTimer.resetTimer();

    while (!glfwWindowShouldClose(getWindow())) {
        glfwPollEvents();
        Editor::beginImGuiDraw(getWindow());

        viewportFramebuffer->bind(); // Render graphics on a separate viewport
        RenderCommand::clear();

        keyboardInput();
        timer.updateDeltaTime();

        emitterTimer.update(timer.getDeltaTime());

        cameraController->update(worldGridShader);

        worldGrid->draw(worldGridShader);

        // Below line must happen after drawing the world grid because of depth
        RenderCommand::clear(GL_DEPTH_BUFFER_BIT);

        cameraController->update(shader);

        //        playerCube->update();
        //        playerCube->draw(shader);

        cameraController->update(emitter->getShader());
        // emitter->update(timer.getDeltaTime());

        // if (emitterTimer.getElapsedTime() > FW::rng(0.15f, 0.5f)) {
        //     emitter->addParticle(FW::rng(5, 15));
        //     emitterTimer.resetTimer();
        // }

        // emitter->draw();
        playerCube->draw(shader);

        // Viewport
        glm::vec2 oldCamSize =
          getCameraController()->getPerspectiveCamera()->getFrustum().getSize();
        glm::vec2 newCamSize = Editor::drawViewport(viewportFramebuffer->getTexture());

        if (newCamSize != oldCamSize) {
            viewportFramebuffer->resize(newCamSize);
            getCameraController()->getPerspectiveCamera()->updateViewportSize(
              newCamSize);
        }

        viewportFramebuffer->unbind();

        // INFO("Show preferences: {0}", widgetState.isSettingsVisible?"true": "false");
        Editor::drawImguiWidgets(getWindow(), &widgetState);

        // Must be called after all other ImGui draw calls
        Editor::endImGuiDraw();

        glfwSwapBuffers(getWindow());
        FW::Input::clearJustPressed();
    }
}

bool PhysicsApp::configureDirectories() {
    std::filesystem::create_directory(RESOURCES_DIR);
    std::filesystem::create_directory(TEXTURES_DIR);
    std::filesystem::create_directory(CONFIG_DIR);

    return true;
}
/** Keyboard input function. Called every frame */
void PhysicsApp::keyboardInput() {
    float speed = cameraSpeed * timer.getDeltaTime();
    // Camera controller spectator mode
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        cameraController->moveForward(speed);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        cameraController->moveForward(-speed);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        cameraController->moveSideway(speed);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        cameraController->moveSideway(-speed);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
        cameraController->moveUp(speed);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
        cameraController->moveUp(-speed);
    }
}

PhysicsApp::~PhysicsApp() {
    FW::TextureManager::clearTextures();
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void PhysicsApp::keyCallback(int key, int scancode, int action, int mods) {
    FW::Input::updateJustPressed(key, action);

    // Update the key press state for left and right SHIFT, ALT, SUPER and
    // CONTROL
    FW::Input::updateModKeyState(key, action);

    if (FW::Input::isKeyJustPressed(FW_KEY_LEFT_SHIFT)) {
        // Saved current mouse cursor
        double xpos, ypos;
        glfwGetCursorPos(getWindow(), &xpos, &ypos);
        savedCursorPosition = glm::vec2(xpos, ypos);
    }

    static bool altBtnJustPressed = false;
    static bool ctrlBtnJustPressed = false;

    double savedX, savedY;
    glfwGetCursorPos(getWindow(), &savedX, &savedY);

    // Quit application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(getWindow(), 1);
    }

    // Camera rotation
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS &&
        !altBtnJustPressed) {
        altBtnJustPressed = true;
        isLeftAltPressed = true;
        getCameraController()->getPerspectiveCamera()->setEnablePanning(false);
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
        altBtnJustPressed = false;
        isLeftAltPressed = false;
        getCameraController()->getPerspectiveCamera()->setEnablePanning(true);
    }

    // Camera panning
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS &&
        !ctrlBtnJustPressed) {
        ctrlBtnJustPressed = true;
        isLeftCtrlPressed = true;
        savedCursorPosition = glm::vec2(savedX, savedY);
        savedCameraPosition = getCameraController()->getPosition();
    }
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
        ctrlBtnJustPressed = false;
        isLeftCtrlPressed = false;
        savedCursorPosition = glm::vec2(savedX, savedY);
        savedCameraPosition = getCameraController()->getPosition();
    }
}
void PhysicsApp::cursorPosCallback(double xpos, double ypos) {
    glm::vec2 diff = glm::vec2((float)xpos, (float)ypos) - savedCursorPosition;
    auto controller = getCameraController();

    float dt = getTimer().getDeltaTime();

    /*
     * Handle camera movement. The following modes are supported by activating
     * mod keys (shift, alt, control) and holding right mouse button:
     *
     * - Shift: Move forward/backward.
     * - Control: Two-dimensional movement based on the currently front vector.
     * - Alt: Orbit around a point.
     * - None: Panning and tilting.
     */
    if (isRightButtonMousePressed) {
        if (FW::Input::isModKeyCombinationPressed(FW_KEY_LEFT_SHIFT_BIT)) {
            // Shift: Move forward/backward
            float mouseDistance = ypos - windowSettings.size.y / 2.0f;
            getCameraController()->moveForward(-mouseDistance * dt * 8.0f);
            centralizeCursorInWindow();
        } else if (FW::Input::isModKeyCombinationPressed(
                     FW_KEY_LEFT_CONTROL_BIT)) {
            // Control: Two-dimensional movement based on the currently front
            // vector.
            // TODO: Fix panning so the direction is dependent on camera front
            controller->setPosition(
              { savedCameraPosition.x + diff.x * 0.1f * dt,
                savedCameraPosition.y - diff.y * 0.1f * dt,
                controller->getPosition().z });

        } else if (FW::Input::isModKeyCombinationPressed(FW_KEY_LEFT_ALT_BIT)) {
            // Alt: Orbit around a point.
            cameraRotationSpeed = 15.0f;

            // Orbit around the camera's focus point based on the distance from
            // where the user mouse clicked to where the cursor is at the moment
            // in screen space.

            // Assume that the distance is in degrees
            glm::vec2 distance = diff * cameraRotationSpeed * 0.01f;

            cameraDistance =
              glm::distance(glm::vec3(0.0f),
                            glm::vec3(controller->getPosition().x,
                                      0.0f,
                                      controller->getPosition().z));

            controller->getPerspectiveCamera()->setEnablePanning(false);

            controller->setPositionX(std::cos(glm::radians(distance.x)) *
                                     cameraDistance);
            controller->setPositionY(std::sin(glm::radians(distance.y)) *
                                     cameraDistance);
            controller->setPositionZ(std::sin(glm::radians(distance.x)) *
                                     cameraDistance);

            // Centralize mouse cursor on screen
            //            centralizeCursorInWindow();
            //            controller->getPerspectiveCamera()->setEnablePanning(true);
        } else {
            // None: Panning and tilting.
            /*
             * Take the distance from the currently saved location when the
             * mouse cursor is clicked (in screen space). Use this metric to
             * determine how much to rotate the camera by.
             */
            float cameraRotationSpeed = 0.05f;
            glm::vec2 difference = glm::vec2(xpos, ypos) - savedCursorPosition;
            auto cam = getCameraController()->getPerspectiveCamera();
            difference.y *= -1;
            glm::vec2 newRotation =
              glm::vec2(cameraCurrentYaw, cameraCurrentPitch) +
              difference * cameraRotationSpeed;

            cam->setRotation(newRotation);
        }
    }
}
void PhysicsApp::mouseButtonCallback(int button, int action, int mods) {
    // Detect the first time the mouse button is pressed;
    struct MouseButtons {
        bool left = false;
        bool middle = false;
        bool right = false;
    };
    static MouseButtons mbtn;

    isLeftButtonMousePressed =
      glfwGetMouseButton(getWindow(), FW_MOUSE_BUTTON_LEFT);

    if (glfwGetMouseButton(getWindow(), FW_MOUSE_BUTTON_RIGHT)) {
        isRightButtonMousePressed = true;

        double x, y;

        if (!mbtn.right) {
            glfwGetCursorPos(getWindow(), &x, &y);
            savedCursorPosition = glm::vec2(x, y);
            cameraCurrentYaw =
              getCameraController()->getPerspectiveCamera()->getYaw();
            cameraCurrentPitch =
              getCameraController()->getPerspectiveCamera()->getPitch();
        }
    } else {
        isRightButtonMousePressed = false;
        mbtn.right = false;
    }

    if (isRightButtonMousePressed) {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}
void PhysicsApp::mouseScrollCallback(double xoffset, double yoffset) {
    if (FW::Input::isKeyPressed(FW_KEY_LEFT_SHIFT) ||
        FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_RIGHT)) {
        setCameraSpeed(getCameraSpeed() + 4.f * yoffset);
        INFO("New camera speed: {}", getCameraSpeed());
    } else {
        getCameraController()->moveForward(yoffset * cameraSpeed * 0.25f);
    }
}
void PhysicsApp::framebufferSizeCallback(int width, int height) {
    // Update the glWindow size
    // setWindowSize({ width, height });
    // getCameraController()->getPerspectiveCamera()->updateViewportSize(
    //   { width, height });
}

void PhysicsApp::configureDefaultEditorSettings() {
    editorConfig = FW::createRef<FW::JSONParser>();

    if (!editorConfig->parse(CONFIG_DIR + std::string("editor.json"))) {
        // TODO Create error dialog to show this error
        FATAL("Failed to configure editor config file");
    }

    editorConfig->get()["ui"]["fontSize"] = 14;

    if (!editorConfig->write(true)) {
        WARN("Failed to write to config/editor.json");
    } else {
        INFO("config/editor.ini write out");
    }
}
