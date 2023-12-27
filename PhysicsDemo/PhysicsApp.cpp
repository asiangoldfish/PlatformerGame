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
    float cameraSpeed = 5.0f * gApp->getTimer().getDeltaTime();

    // Move camera rotation
    if (gApp && gApp->getIsRightButtonPressed()) {
        // Cursor is hidden and within window bounds

        // Make sure mouse is within bounds
        if (xpos < 0) {
            xpos = 0;
        } else if (xpos > gApp->getWindowSize().x) {
            xpos = gApp->getWindowSize().x;
        }

        if (ypos < 0) {
            ypos = 0;
        } else if (ypos > gApp->getWindowSize().y) {
            ypos = gApp->getWindowSize().y;
        }

        glfwGetCursorPos(window, &xpos, &ypos);
        glm::vec2 rotation = glm::vec2{ xpos - gApp->getWindowSize().x / 2,
                                        ypos - gApp->getWindowSize().y / 2 } *
                             cameraSpeed;

        gApp->getCameraController()->rotate({ rotation.x, -rotation.y });

        glfwSetCursorPos(
          window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
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

    gApp->setIsRightButtonPressed(
      glfwGetMouseButton(window, MOUSE_RIGHT_CLICK));

    if (gApp->getIsRightButtonPressed()) {
        glfwSetCursorPos(window,
                         gApp->getWindowSize().x / 2.0f,
                         gApp->getWindowSize().y / 2.0f);
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
    // Quit application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
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
