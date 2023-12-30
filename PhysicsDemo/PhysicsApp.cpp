/**
 * Copyright 2023 Khai Duong
 */

#include "PhysicsApp.h"

// Framework API
#include "Framework.h"

// App Components
#include "Cube.h"
#include "Floor.h"
#include "WorldGrid.h"

PhysicsApp* gApp = nullptr;

// Function prototypes
static void
cursorPos_callback(GLFWwindow* window, double xpos, double ypos);

static void
cursorMouseButton_callback(GLFWwindow* window, int, int, int);

static void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);

static void
mouseScrollBack_callback(GLFWwindow* window, double xoffset, double yoffset);

static void
framebufferSize_callback(GLFWwindow* window, int width, int height);

bool
PhysicsApp::init()
{
    // ------
    // Configure application
    // ------
    if (!GLFWApplication::init()) {
        return false;
    }

    gApp =
      this; // This needs to happen before anything else to prevent segfault
    glfwSetKeyCallback(getWindow(), keyCallback);
    glfwSetCursorPosCallback(getWindow(), cursorPos_callback);
    glfwSetMouseButtonCallback(getWindow(), cursorMouseButton_callback);
    glfwSetScrollCallback(getWindow(), mouseScrollBack_callback);
    glfwSetFramebufferSizeCallback(getWindow(), framebufferSize_callback);

    INFO("GLFW callback functions successfully bound");

    // -----------
    // Textures and shaders
    // -----------
    shader = FW::createRef<FW::Shader>(
      RESOURCES_DIR + std::string("shaders/vertex.glsl"),
      RESOURCES_DIR + std::string("shaders/fragment.glsl"));

    worldGridShader = FW::createRef<FW::Shader>(
      RESOURCES_DIR + std::string("shaders/worldGridVertex.glsl"),
      RESOURCES_DIR + std::string("shaders/worldGridFrag.glsl"));

    FW::TextureManager::loadTexture2D(
      { { "metal_plate_diff",
          TEXTURES_DIR +
            std::string("polyhaven/metal_plate/metal_plate_diff_1k.jpg") },
        { "metal_plate_spec",
          TEXTURES_DIR +
            std::string("polyhaven/metal_plate/metal_plate_spec_1k_GIMP.jpg") },
        { "coral_stone_diff",
          TEXTURES_DIR +
            std::string("polyhaven/coral_stone/coral_stone_wall_diff_1k.jpg") },
        { "coral_stone_spec",
          TEXTURES_DIR +
            std::string("polyhaven/coral_stone/coral_stone_wall_diff_1k.jpg") },
        { "wall", TEXTURES_DIR + std::string("wall.jpg") } });

    // We only have one shader in the application, so we only bind it here.
    shader->bind();
    getShader()->setInt("u_material.diffuse", 0);
    getShader()->setInt("u_material.specular", 1);

    // -------------
    // Entities
    // -------------
    FW::scope<FW::Entity> playerCube = FW::createScope<Cube>();
    //    playerCube->setScale(0.8f);
    playerCube->setPosition({ 0, 1.f, 0 });
    playerCube->setScale({ 1.0f, 2.0f, 1.0f });
    playerCube->recalculateModelMatrix();
    playerCube->getMaterial().getProperties().diffuseTextureId =
      FW::TextureManager::getTextureID("metal_plate_diff");
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

    INFO("Client application successfully initialized");
    shader->bind();

    worldGrid = FW::createRef<WorldGrid>();

    return true;
}

void
PhysicsApp::run()
{
    while (!glfwWindowShouldClose(getWindow())) {
        RenderCommand::clear();
        glfwPollEvents();
        keyboardInput();
        timer.updateDeltaTime();

        cameraController->update(worldGridShader);
        worldGrid->draw(worldGridShader);

        RenderCommand::clear(GL_DEPTH_BUFFER_BIT);

        cameraController->update(shader);
        shader->setFloat3("u_cameraPosition",
                          getCameraController()->getPosition());

        auto playerCube = playerController->getPossessedEntity();
        playerCube->update();
        playerCube->draw(shader);

        glfwSwapBuffers(getWindow());
        FW::Input::clearJustPressed();
    }
}

/** Keyboard input function. Called every frame */
void
PhysicsApp::keyboardInput()
{
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

PhysicsApp::~PhysicsApp()
{
    FW::TextureManager::clearTextures();
}

// Source: https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
void
cursorPos_callback(GLFWwindow* window, double xpos, double ypos)
{
    glm::vec2 diff =
      glm::vec2((float)xpos, (float)ypos) - gApp->savedCursorPosition;
    auto controller = gApp->getCameraController();

    float dt = gApp->getTimer().getDeltaTime();
    float cameraSpeed = 5.0f * dt;

    // Move camera rotation
    if (gApp && gApp->isRightButtonMousePressed) {
        if (FW::Input::isKeyPressed(FW_KEY_LEFT_SHIFT) &&
            !FW::Input::isKeyPressed(FW_KEY_LEFT_ALT) &&
            !FW::Input::isKeyPressed(FW_KEY_LEFT_CONTROL)) {
            // Only left shift is pressed
            float mouseDistance = ypos - gApp->getWindowSize().y / 2.0f;
            gApp->getCameraController()->moveForward(-mouseDistance * dt *
                                                     8.0f);
            gApp->centralizeCursorInWindow();
        } else if (gApp->isLeftCtrlPressed) {
            // Only left ctrl is pressed
            // TODO: Fix panning so the direction is dependent on camera front
            controller->setPosition(
              { gApp->savedCameraPosition.x + diff.x * 0.1f * dt,
                gApp->savedCameraPosition.y - diff.y * 0.1f * dt,
                controller->getPosition().z });
        } else if (gApp->isLeftAltPressed && !gApp->isLeftCtrlPressed &&
                   !gApp->isLeftShiftPressed) {
            // Only alt is pressed
            gApp->cameraRotationSpeed = 15.0f;

            // Orbit around the camera's focus point based on the distance from
            // where the user mouse clicked to where the cursor is at the moment
            // in screen space.

            // Assume that the distance is in degrees
            glm::vec2 distance =
              glm::vec2{ diff.x * dt * gApp->cameraRotationSpeed,
                         diff.y * dt * gApp->cameraRotationSpeed };

            //            gApp->cameraDistance = 5.0f;

            gApp->cameraDistance =
              glm::distance(glm::vec3(0.0f),
                            glm::vec3(controller->getPosition().x,
                                      0.0f,
                                      controller->getPosition().z));

            controller->getPerspectiveCamera()->setEnablePanning(false);

            controller->setPositionX(std::cos(glm::radians(distance.x)) *
                                     gApp->cameraDistance);
            controller->setPositionY(std::sin(glm::radians(distance.y)) *
                                     gApp->cameraDistance);
            controller->setPositionZ(std::sin(glm::radians(distance.x)) *
                                     gApp->cameraDistance);

            // Centralize mouse cursor on screen
            //            gApp->centralizeCursorInWindow();
            //            controller->getPerspectiveCamera()->setEnablePanning(true);
        } else {
            // No mod key is pressed
            /*
             * Take the distance from the currently saved location when the
             * mouse cursor is clicked (in screen space). Use this metric to
             * determine how much to rotate the camera by.
             */
            float cameraRotationSpeed = 0.05f;
            glm::vec2 difference =
              glm::vec2(xpos, ypos) - gApp->savedCursorPosition;
            auto cam = gApp->getCameraController()->getPerspectiveCamera();
            difference.y *= -1;
            glm::vec2 newRotation =
              glm::vec2(gApp->cameraCurrentYaw, gApp->cameraCurrentPitch) +
              difference * cameraRotationSpeed;

            cam->setRotation(newRotation);
        }
    }
}

void
mouseScrollBack_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    //    gApp->getCameraController()->moveForward(yoffset * 0.5f);
    gApp->setCameraSpeed(gApp->getCameraSpeed() + 4.f * yoffset);
    INFO("New camera speed: {}", gApp->getCameraSpeed());
}

// Source: https://www.glfw.org/docs/3.0/group__input.html
void
cursorMouseButton_callback(GLFWwindow* window, int, int, int)
{
    if (!gApp) {
        return;
    }

    // Detect the first time the mouse button is pressed;
    struct MouseButtons
    {
        bool left = false;
        bool middle = false;
        bool right = false;
    };
    static MouseButtons mbtn;

    gApp->isLeftButtonMousePressed =
      glfwGetMouseButton(window, FW_MOUSE_BUTTON_LEFT);

    if (glfwGetMouseButton(window, FW_MOUSE_BUTTON_RIGHT)) {
        gApp->isRightButtonMousePressed = true;

        double x, y;

        if (!mbtn.right) {
            glfwGetCursorPos(window, &x, &y);
            gApp->savedCursorPosition = glm::vec2(x, y);
            gApp->cameraCurrentYaw =
              gApp->getCameraController()->getPerspectiveCamera()->getYaw();
            gApp->cameraCurrentPitch =
              gApp->getCameraController()->getPerspectiveCamera()->getPitch();
        }
    } else {
        gApp->isRightButtonMousePressed = false;
        mbtn.right = false;
    }

    if (gApp->isRightButtonMousePressed) {
        //        glfwSetCursorPos(window,
        //                         gApp->getWindowSize().x / 2.0f,
        //                         gApp->getWindowSize().y / 2.0f);
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
}

/* Keyboard input function. Used as callback function and is used in
 * OpenGL's input polling system. */
void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    FW::Input::updateJustPressed(key, action);

    if (FW::Input::isKeyJustPressed(FW_KEY_LEFT_SHIFT)) {
        // Saved current mouse cursor
        double xpos, ypos;
        glfwGetCursorPos(gApp->getWindow(), &xpos, &ypos);
        gApp->savedCursorPosition = glm::vec2(xpos, ypos);
    }

    static bool altBtnJustPressed = false;
    static bool ctrlBtnJustPressed = false;

    double savedX, savedY;
    glfwGetCursorPos(window, &savedX, &savedY);

    // Quit application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    // Camera rotation
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_PRESS &&
        !altBtnJustPressed) {
        altBtnJustPressed = true;
        gApp->isLeftAltPressed = true;
        gApp->getCameraController()->getPerspectiveCamera()->setEnablePanning(
          false);
    }
    if (key == GLFW_KEY_LEFT_ALT && action == GLFW_RELEASE) {
        altBtnJustPressed = false;
        gApp->isLeftAltPressed = false;
        gApp->getCameraController()->getPerspectiveCamera()->setEnablePanning(
          true);
    }

    // Camera panning
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_PRESS &&
        !ctrlBtnJustPressed) {
        ctrlBtnJustPressed = true;
        gApp->isLeftCtrlPressed = true;
        gApp->savedCursorPosition = glm::vec2(savedX, savedY);
        gApp->savedCameraPosition = gApp->getCameraController()->getPosition();
    }
    if (key == GLFW_KEY_LEFT_CONTROL && action == GLFW_RELEASE) {
        ctrlBtnJustPressed = false;
        gApp->isLeftCtrlPressed = false;
        gApp->savedCursorPosition = glm::vec2(savedX, savedY);
        gApp->savedCameraPosition = gApp->getCameraController()->getPosition();
    }
}

void
framebufferSize_callback(GLFWwindow* window, int width, int height)
{
    // Update the glWindow size
    gApp->setWindowSize({ width, height });
    gApp->getCameraController()->getPerspectiveCamera()->updateViewportSize(
      { width, height });
}
