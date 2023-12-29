/**
 * Copyright 2023 Khai Duong
 */

#include "PhysicsApp.h"

// Framework API
#include "Framework.h"

// App Components
#include "Cube.h"
#include "Floor.h"

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
    playerCube->setPosition({ 0, 0.5f, 0 });
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
    cameraController->rotate({ -90.0f, -45.0f });
    cameraController->setPosition({ 0.0f, 5.0f, 5.0f });
    cameraController->setSpectatorMode(true);

    // Screen
    RenderCommand::setClearColor(glm::vec3(0.5f, 0.5f, 0.5f));

    testCube = FW::createRef<Cube>();
    testCube->setPosition({ 0.0f, 0.0f, -3.0f });
    testCube->setScale(1.0f);

    // Grid
    int gridSize = 10;
    grid = FW::createScope<Floor>(gridSize);
    grid->setRotation({ 90.0f, 0.0f, 0.0f });
    grid->setTiles(10, 10);
    grid->setPosition({ -gridSize / 2, 0, -gridSize / 2 });

    RenderCommand::setClearColor(glm::vec3{ 0.2f, 0.1f, 0.215f });
    shader->setVisualizeMode(RenderCommand::VisualizeMode::NORMAL);

    INFO("Client application successfully initialized");
    shader->bind();

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

        RenderCommand::setPolygonMode(RenderCommand::PolygonMode::WIREFRAME);
        grid->draw(shader);
        RenderCommand::setPolygonMode(RenderCommand::PolygonMode::SOLID);

        cameraController->update(shader);
        shader->setFloat3("u_cameraPosition",
                          getCameraController()->getPosition());

        auto playerCube = playerController->getPossessedEntity();

        playerCube->update();
        playerCube->draw(shader);

        glfwSwapBuffers(getWindow());
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
        // Cursor is hidden and within window bounds

        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 rotation = glm::vec2{ xpos - gApp->getWindowSize().x / 2,
                                        ypos - gApp->getWindowSize().y / 2 } *
                             cameraSpeed;

        if (gApp->isLeftAltPressed && gApp->isLeftCtrlPressed &&
            !gApp->isLeftShiftPressed) {
            // Both alt and ctrl are pressed
        } else if (gApp->isLeftShiftPressed && !gApp->isLeftCtrlPressed &&
                   !gApp->isLeftAltPressed) {
            // Only shift is pressed
            gApp->getCameraController()->moveForward(-rotation.y * dt * 8.0f);
            glfwSetCursorPos(
              window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
        } else if (gApp->isLeftCtrlPressed) {
            // Only ctrl is pressed
            // TODO: Fix panning so the direction is dependent on camera front
            controller->setPosition(
              { gApp->savedCameraPosition.x + diff.x * 0.1f * dt,
                gApp->savedCameraPosition.y - diff.y * 0.1f * dt,
                controller->getPosition().z });
        } else if (gApp->isLeftAltPressed && !gApp->isLeftCtrlPressed &&
                   !gApp->isLeftShiftPressed) {
            // Only alt is pressed
            gApp->cameraRotationSpeed = 15.0f;
            float rotX = rotation.x * dt * gApp->cameraRotationSpeed;
            float rotY = rotation.y * dt * gApp->cameraRotationSpeed;

            gApp->cameraDegreesX += rotX;
            gApp->cameraDegreesY += rotY;
            gApp->cameraDistance = 5.0f;

            INFO("Yaw: {} | DegreesX: {}", controller->getPerspectiveCamera()->getYaw(), gApp->cameraDegreesX);

            controller->getPerspectiveCamera()->setEnablePanning(false);
            //            controller->setPositionY(controller->getPosition().y +
            //            rotY);

            controller->setPositionX(std::cos(glm::radians(gApp->cameraDegreesX)) * gApp->cameraDistance);
            controller->setPositionZ(std::sin(glm::radians(gApp->cameraDegreesX)) * gApp->cameraDistance);
            controller->setPositionY(std::sin(glm::radians(gApp->cameraDegreesY)) * gApp->cameraDistance);

            // Centralize mouse cursor on screen
            glfwSetCursorPos(
              window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
            controller->getPerspectiveCamera()->setEnablePanning(true);
        } else {
            // No mod key is pressed
            gApp->getCameraController()->rotate(
              { rotation.x * 0.5f, -rotation.y * 0.5f });
            glfwSetCursorPos(
              window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
        }
    }

    //    if (gApp && gApp->isLeftAltPressed && gApp->isLeftButtonMousePressed)
    //    {
    //        //        auto currentCameraRotation =
    //        //        gApp->getCameraController()->getSelectedCamera()->get;
    //        glfwGetCursorPos(window, &xpos, &ypos);
    //        glm::vec2 rotation = glm::vec2{ xpos - gApp->getWindowSize().x /
    //        2,
    //                                        ypos - gApp->getWindowSize().y / 2
    //                                        } *
    //                             cameraSpeed;
    //
    //        gApp->getCameraController()->rotate({ rotation.x, -rotation.y });
    //    }
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

    gApp->isRightButtonMousePressed =
      glfwGetMouseButton(window, MOUSE_RIGHT_CLICK);
    gApp->isLeftButtonMousePressed =
      glfwGetMouseButton(window, MOUSE_LEFT_CLICK);

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
    static bool altBtnJustPressed = false;
    static bool ctrlBtnJustPressed = false;
    static bool shiftBtnJustPressed = false;

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

    // Camera zoom
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS &&
        !shiftBtnJustPressed) {
        shiftBtnJustPressed = true;
        gApp->isLeftShiftPressed = true;
    }
    if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_RELEASE) {
        shiftBtnJustPressed = false;
        gApp->isLeftShiftPressed = false;
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
