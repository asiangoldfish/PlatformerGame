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
    player = FW::createRef<Cube>();
    player->setScale(0.8f);
    player->setPosition({ 2, -5, 1 });
    player->getMaterial().getProperties().diffuseTextureId =
      FW::TextureManager::getTextureID("metal_plate_diff");
    player->setMaterial(FW::MaterialPreset::CHROME);

    // ---------
    // Rendering
    // ---------
    // Configure camera
    cameraController =
      FW::createRef<FW::CameraController>(FW::CameraType::PERSPECTIVE);
    cameraController->getPerspectiveCamera()->setEnablePanning(true);
    cameraController->setPosition({ 6.0f, -4.0f, 7.0f });
    cameraController->rotate({ -90.0f, 0.0f });

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

        cameraController->setPosition({ player->getPosition().x,
                                        player->getPosition().y + 0.25f,
                                        cameraController->getPosition().z });
        player->draw(shader);

        glfwSwapBuffers(getWindow());
    }
}

/** Keyboard input function. Called every frame */
void
SokobanApplication::keyboardInput()
{
    float moveBy = timer.getDeltaTime();

    // ----------
    // Player movement
    // ----------
    float playerMovementSpeed = moveBy * 3.f;
    if (glfwGetKey(getWindow(), GLFW_KEY_UP) == GLFW_PRESS) {
        player->move({ 0, playerMovementSpeed, 0 });
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_DOWN) == GLFW_PRESS) {
        player->move({ 0, -playerMovementSpeed, 0 });
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_RIGHT) == GLFW_PRESS) {
        player->move({ playerMovementSpeed, 0, 0 });
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_LEFT) == GLFW_PRESS) {
        player->move({ -playerMovementSpeed, 0, 0 });
    }

    // ----------
    // Camera movement
    // ----------
    /*
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        cameraController->moveForward(moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        cameraController->moveForward(-moveBy);
    }

    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        cameraController->moveSideway(moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        cameraController->moveSideway(-moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
        cameraController->moveUp(moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
        cameraController->moveUp(-moveBy);
    }
    */
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
    gApp->getCameraController()->moveForward(yoffset * 0.1f);
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

    // Toggle textures
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        gApp->setEnableTexture(!gApp->getEnableTexture());
        gApp->getShader()->setInt("u_enableTexture", gApp->getEnableTexture());
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
