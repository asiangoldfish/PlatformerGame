/**
 * Copyright 2023-2024 Khai Duong
 */

#include "PhysicsApp.h"

// Framework API
#include "Framework.h"

// App Components
#include "WorldGrid.h"

void setCameraPositionAndYaw(GLFWwindow* window,
                             glm::vec2& savedCursorPosition,
                             FW::PerspectiveCamera& camera,
                             float& pitch,
                             float& yaw);

bool PhysicsApp::init() {
    // ------
    // Configure application
    // ------
    if (!GLFWApplication::init()) {
        return false;
    }

    editorConfig = FW::createRef<FW::JSONParser>();

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
    shader->setInt("u_material.diffuse", 0);
    shader->setInt("u_material.specular", 1);

    // -------------
    // Entities
    // -------------
    FW::TextureManager::loadTexture2D(
      "metal_plate_diff",
      RESOURCES_DIR +
        std::string("textures/polyhaven/metal_plate/metal_plate_diff_1k.jpg"));

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

    RenderCommand::setClearColor(glm::vec3{ 0.2f, 0.1f, 0.215f });
    shader->setVisualizeMode(RenderCommand::VisualizeMode::NORMAL);

    worldGrid = FW::createRef<WorldGrid>();

    // Init Dear ImGui
    appWidget.init(getWindow());
    appWidget.editorConfig = editorConfig;
    appWidget.setFontSize(editorConfig->get()["ui"]["fontSize"]);

    // Framebuffer for rendering GL on ImGui
    viewportFramebuffer = FW::createRef<FW::Framebuffer>();

    scene = FW::createRef<PhysicsScene>();
    scene->setShader(shader);
    scene->init();

    INFO("Client application successfully initialized");

    return true;
}

void PhysicsApp::run() {
    FW::Timer emitterTimer;
    emitterTimer.resetTimer();

    while (!glfwWindowShouldClose(getWindow())) {
        glfwPollEvents();
        appWidget.beginDraw();

        viewportFramebuffer->bind(); // Render graphics on a separate viewport
        RenderCommand::clear();

        keyboardInput();
        timer.updateDeltaTime();

        emitterTimer.update(timer.getDeltaTime());

        cameraController->update(worldGridShader);

        worldGrid->draw(worldGridShader);

        // Below line must happen after drawing the world grid because of depth
        RenderCommand::clear(GL_DEPTH_BUFFER_BIT);

        cameraController->update(scene->getShader());

        // Viewport
        glm::vec2 oldCamSize =
          getCameraController()->getPerspectiveCamera()->getFrustum().getSize();
        glm::vec2 newCamSize = appWidget.drawViewport(viewportFramebuffer->getTexture());

        if (newCamSize != oldCamSize) {
            viewportFramebuffer->resize(newCamSize);
            getCameraController()->getPerspectiveCamera()->updateViewportSize(
              newCamSize);
        }

        // Because the GLFW application does not register mouse events in ImGui
        // widgets by default, we should handle it ourselves.
        overrideRightMouseDown = appWidget.mouseState.isRightButtonDown;
        // BUG when moving rotating after forcing right mouse button down,
        // the camera snaps a bit every time we right click.
        if (!isRightButtonMousePressed) {
            cursorPosCallback(appWidget.mouseState.mousePosition.x,
                appWidget.mouseState.mousePosition.y);
        }

        scene->update(timer.getDeltaTime());
        appWidget.drawSceneTree(scene);

        appWidget.drawWidgets();
        
        viewportFramebuffer->unbind();
        appWidget.endDraw();

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
}

void PhysicsApp::keyCallback(int key, int scancode, int action, int mods) {
    FW::Input::updateJustPressed(key, action);

    // Update the key press state for left and right SHIFT, ALT, SUPER and
    // CONTROL
    FW::Input::updateModKeyState(key, action);

    // if (FW::Input::isKeyJustPressed(FW_KEY_LEFT_SHIFT)) {
    //     // Saved current mouse cursor
    //     double xpos, ypos;
    //     glfwGetCursorPos(getWindow(), &xpos, &ypos);
    //     savedCursorPosition = glm::vec2(xpos, ypos);
    // } 
    // else 
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        setCameraPositionAndYaw(
            getWindow(),
            savedCursorPosition,
            *getCameraController()->getPerspectiveCamera(),
            cameraCurrentPitch,
            cameraCurrentYaw);
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
        
        setCameraPositionAndYaw(
            getWindow(),
            savedCursorPosition,
            *getCameraController()->getPerspectiveCamera(),
            cameraCurrentPitch,
            cameraCurrentYaw);
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

    // Window shortcuts
    if (key == GLFW_KEY_P && action == GLFW_PRESS) {
        appWidget.widgetStates.editorPreferences = !appWidget.widgetStates.editorPreferences;
    }
}
void PhysicsApp::cursorPosCallback(double xpos, double ypos) {
    auto controller = getCameraController();
    auto camera = controller->getPerspectiveCamera();
    glm::vec2 difference = glm::vec2(xpos, ypos) - savedCursorPosition;
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
    if (isRightButtonMousePressed || overrideRightMouseDown) {
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
              { camera->getPosition().x + difference.x * 0.1f * dt,
                camera->getPosition().y - difference.y * 0.1f * dt,
                controller->getPosition().z });

            glfwSetCursorPos(getWindow(), savedCursorPosition.x, savedCursorPosition.y);

        } else if (FW::Input::isModKeyCombinationPressed(FW_KEY_LEFT_ALT_BIT)) {
            // Alt: Orbit around a point.
            cameraRotationSpeed = 15.0f;

            // Orbit around the camera's focus point based on the distance from
            // where the user mouse clicked to where the cursor is at the moment
            // in screen space.

            // Assume that the distance is in degrees
            glm::vec2 distance = difference * cameraRotationSpeed * 0.01f;

            cameraDistance =
              glm::distance(glm::vec3(0.0f),
                            glm::vec3(controller->getPosition().x,
                                      0.0f,
                                      controller->getPosition().z));

            camera->setEnablePanning(false);
                
            controller->setPositionX(std::cos(glm::radians(distance.x)) *
                                        cameraDistance);
            controller->setPositionY(std::sin(glm::radians(distance.y)) *
                                        cameraDistance);
            controller->setPositionZ(std::sin(glm::radians(distance.x)) *
                                        cameraDistance);
                                        
            // ---------------------
            // Current development

            float x = -getCameraController()->getPosition().x;
            float y = -getCameraController()->getPosition().y;
            float z = -getCameraController()->getPosition().z;

            // https://stackoverflow.com/a/33790309
            glm::vec3 d = glm::normalize(glm::vec3(x, y, z));
            float pitch = glm::degrees(asin(d.y));
            float yaw = glm::degrees(atan2(d.z, d.x));
            // camera->setRotation({ yaw, pitch });

            getCameraController()->getPerspectiveCamera()->setRotation({yaw, pitch});
            
        } else {
            // None: Panning and tilting.
            /*
             * Take the distance from the currently saved location when the
             * mouse cursor is clicked (in screen space). Use this metric to
             * determine how much to rotate the camera by.
             */
            float cameraRotationSpeed = 0.05f;

            difference.y *= -1;
            glm::vec2 newRotation =
              glm::vec2(cameraCurrentYaw, cameraCurrentPitch) +
              difference * cameraRotationSpeed;

            camera->setRotation(newRotation);
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

        if (!mbtn.right) {
            setCameraPositionAndYaw(
              getWindow(),
              savedCursorPosition,
              *getCameraController()->getPerspectiveCamera(),
              cameraCurrentPitch,
              cameraCurrentYaw);
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

/**
 * We use this function whenever we want to move the camera. It should be used
 * at the beginning of moving the mouse, and at the end.
 *
 * This is a helper function.
 */
void setCameraPositionAndYaw(GLFWwindow* window,
                             glm::vec2& savedCursorPosition,
                             FW::PerspectiveCamera& camera,
                             float& pitch,
                             float& yaw) {

    double x, y;
    glfwGetCursorPos(window, &x, &y);
    savedCursorPosition = glm::vec2(x, y);
    yaw = camera.getYaw();
    pitch = camera.getPitch();
}
