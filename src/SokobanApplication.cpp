/**
 * Copyright 2023 Khai Duong
 */

#include "SokobanApplication.h"

// Framework API
#include "Framework.h"

// App Components
#include "Cube.h"
#include "Map.h"

SokobanApplication* gApp = nullptr;

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
SokobanApplication::init()
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

    skyboxShader = FW::createRef<FW::Shader>(
      RESOURCES_DIR + std::string("shaders/skyboxVertex.glsl"),
      RESOURCES_DIR + std::string("shaders/skyboxFrag.glsl"));

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
    getShader()->setInt("u_enableTexture", getEnableTexture());
    getShader()->setInt("u_material.diffuse", 0);
    getShader()->setInt("u_material.specular", 1);

    // ------------
    // Map
    // ------------
    map = FW::createRef<Map>();
    map->addNewMap("test", RESOURCES_DIR + std::string("/maps/level.map"));
    map->loadMap("test");

    // -------------
    // Entities
    // -------------
    FW::scope<FW::Entity> playerCube = FW::createScope<Cube>();
    //    playerCube->setScale(0.8f);
    playerCube->setPosition({ 2, 3, 0 });
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
    cameraController->rotate({ -90.0f, 0.0f });
    cameraController->setPosition(
      { playerController->getPossessedEntity()->getPosition().x + 3.0f,
        playerController->getPossessedEntity()->getPosition().y + 2.0f,
        15.0f });
    cameraController->setCameraOffset({ 3.0f, 2.0f, 0.0f });

    // Screen
    RenderCommand::setClearColor(glm::vec3(0.5f, 0.5f, 0.5f));

    // -------------
    // Lighting
    // -------------
    sun.setShader(shader);
    sun.setAmbient(glm::vec3{ 0.3f, 0.4f, 0.8f });
    sun.setDiffuse(glm::vec3{ 0.6f });
    sun.setSpecular(glm::vec3{ 0.0f });
    sun.setDirection({ 0.0f, -1.0f, -1.f });

    pointLight.setShader(shader);
    pointLight.setAmbient(glm::vec3{ 0.5f });
    pointLight.setDiffuse(glm::vec3{ 0.4f, 0.5f, 0.8f });
    pointLight.setSpecular(glm::vec3{ 0.2f });
    pointLight.setPosition(glm::vec3{ 5.0f, -4.0f, 5.0f });
    pointLight.setLinear(0.45f);
    pointLight.setQuadratic(0.07f);
    pointLight.setBrightness(3.0f);

    testCube = FW::createRef<Cube>();
    testCube->setPosition({ 0.0f, 0.0f, -3.0f });
    testCube->setScale(1.0f);

    // Skybox
    auto faces = {
        TEXTURES_DIR + std::string("skybox/demo/right.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/left.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/bottom.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/top.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/front.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/back.jpg"),
    };
    int skyboxId = FW::TextureManager::loadCubeMap("skybox_demo", faces);
    skybox = FW::createRef<FW::Skybox>(skyboxId);
    skybox->setScale(700.0f);

    RenderCommand::setClearColor(glm::vec3{ 0.2f, 0.1f, 0.215f });
    shader->setVisualizeMode(RenderCommand::VisualizeMode::NORMAL);

    INFO("Client application successfully initialized");
    return true;
}

void
SokobanApplication::run()
{
    while (!glfwWindowShouldClose(getWindow())) {
        RenderCommand::clear();
        glfwPollEvents();
        keyboardInput();
        timer.updateDeltaTime();

        cameraController->setFarClip(701.0f);
        cameraController->update(skyboxShader);
        skybox->draw(skyboxShader);

        cameraController->setFarClip(100.0f);

        shader->bind();
        cameraController->update(shader);
        shader->setFloat3("u_cameraPosition",
                          getCameraController()->getPosition());

        // Sun
        sun.draw(shader);

        // --------
        // Camera uploads
        // --------
        map->update();
        map->draw(shader);

        auto playerCube = playerController->getPossessedEntity();

        //        cameraController->setPosition({ playerCube->getPosition().x,
        //                                        playerCube->getPosition().y +
        //                                        0.25f,
        //                                        cameraController->getPosition().z
        //                                        });

        playerCube->update();
        playerCube->draw(shader);
        if (!cameraController->getSpectatorMode()) {
            cameraController->setPosition(
              glm::vec3(playerCube->getPosition().x,
                        playerCube->getPosition().y,
                        cameraController->getPosition().z));
        }

        glfwSwapBuffers(getWindow());
    }
}

/** Keyboard input function. Called every frame */
void
SokobanApplication::keyboardInput()
{
    float delta = timer.getDeltaTime();
    static bool canJump = true;
    auto playerEntity = playerController->getPossessedEntity();
    auto playerBox = playerController->getPossessedEntity()->getBoundingBox();
    static float velocity = 0;
    float playerMovementSpeed = delta * 6.f;
    float gravity = 0.5f * delta;
    float jumpingPower = 5.0f;
    bool canPlayerMoveRight = true;
    bool canPlayerMoveLeft = true;
    float movementSpeed = 0.0f;
    float positionY = playerEntity->getPosition().y;

    std::vector<FW::BoundingBox_Quad*> collidingBoxes;
    for (auto& child : map->getBaseNode()->getChildren()) {
        collidingBoxes.push_back(&child->getBoundingBox());
    }

    // Walk sideways
    if (glfwGetKey(getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        for (const auto& node : map->getBaseNode()->getChildren()) {
            if (!FW::isOverlapping(playerBox, node->getBoundingBox())) {
                auto targetBox = node->getBoundingBox();
                if (
                  // Check right side is inside target box
                  playerBox.maxX + playerMovementSpeed >= targetBox.minX &&
                  playerBox.maxX <= targetBox.maxX &&
                  playerBox.minY < targetBox.maxY &&
                  playerBox.minY >= targetBox.minY) {
                    canPlayerMoveRight = false;
                    break;
                }
            }
        }

        if (canPlayerMoveRight) {
            movementSpeed = 1.0f;
        }
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        for (const auto& node : map->getBaseNode()->getChildren()) {
            if (!FW::isOverlapping(playerBox, node->getBoundingBox())) {
                auto targetBox = node->getBoundingBox();
                if (
                  // Check left side is inside target box
                  playerBox.minX - playerMovementSpeed <= targetBox.maxX &&
                  playerBox.minX >= targetBox.minX &&
                  playerBox.minY < targetBox.maxY &&
                  playerBox.minY >= targetBox.minY) {
                    canPlayerMoveLeft = false;
                    float distance = playerBox.minX - targetBox.maxX;
                    if (distance == 0.0f) {
                        movementSpeed = distance;
                    }
                    break;
                }
            }
        }

        if (canPlayerMoveLeft) {
            movementSpeed = -1.0f;
        }
    }

    // Collision detection top
    bool isTopColliding = false;
    FW::BoundingBox_Quad* collidedBox = nullptr;
    for (const auto& node : map->getBaseNode()->getChildren()) {
        if (FW::isOverlapping(playerBox, node->getBoundingBox())) {
            if (playerBox.minY <= node->getBoundingBox().maxY) {
                isTopColliding = true;
                velocity = 0.0f;
            }
            break;
        }
    }

    if (glfwGetKey(getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        if (canJump) {
            velocity += jumpingPower * delta;
            canJump = false;
        }
    }

    if (isTopColliding) {
        canJump = true;
    } else {
        velocity -= gravity;
        canJump = false;
    }

    positionY += velocity;

    playerEntity->setPosition(
      { playerEntity->getPosition().x + playerMovementSpeed * movementSpeed,
        positionY,
        0.0f });

    // Camera controller spectator mode
    float cameraSpeed = 5.0f * delta;
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        if (cameraController->getSpectatorMode()) {
            cameraController->setPosition(
              { cameraController->getPosition().x,
                cameraController->getPosition().y + cameraSpeed,
                cameraController->getPosition().z });
        }
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        if (cameraController->getSpectatorMode()) {
            cameraController->setPosition(
              { cameraController->getPosition().x,
                cameraController->getPosition().y - cameraSpeed,
                cameraController->getPosition().z });
        }
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        if (cameraController->getSpectatorMode()) {
            cameraController->setPosition(
              { cameraController->getPosition().x + cameraSpeed,
                cameraController->getPosition().y,
                cameraController->getPosition().z });
        }
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        if (cameraController->getSpectatorMode()) {
            cameraController->setPosition(
              { cameraController->getPosition().x - cameraSpeed,
                cameraController->getPosition().y,
                cameraController->getPosition().z });
        }
    }
}

SokobanApplication::~SokobanApplication()
{
    FW::TextureManager::clearTextures();
}

// Source: https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
void
cursorPos_callback(GLFWwindow* window, double xpos, double ypos)
{
    /*
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
        glm::vec2 rotation =
          glm::vec2{ xpos - gApp->getWindowSize().x / 2,
                     ypos - gApp->getWindowSize().y / 2 } *
          glm::vec2(gApp->getCameraController()->getRotationSpeed());

        gApp->getCameraController()->rotate({ rotation.x, -rotation.y });

        glfwSetCursorPos(
          window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
    }
     */
}

void
mouseScrollBack_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gApp->getCameraController()->moveForward(yoffset * 0.5f);
}

// Source: https://www.glfw.org/docs/3.0/group__input.html
void
cursorMouseButton_callback(GLFWwindow* window, int, int, int)
{
    /*
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
     */
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
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        auto controller = gApp->getCameraController();
        controller->setSpectatorMode(!controller->getSpectatorMode());

        if (controller->getSpectatorMode()) {
            INFO("Spectator enabled");
        } else {
            INFO("Spectator disabled");
        }
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
