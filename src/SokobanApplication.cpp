/**
 * Copyright 2023 Khai Duong
 */

#include "SokobanApplication.h"

// Standard libraries
#include <algorithm>
#include <string>

// Framework API
#include "Framework.h"

// App Components
#include "Cube.h"
#include "Floor.h"
#include "Map.h"
#include "gameMath.h"

#define TIMEOFDAY glm::radians((float)glfwGetTime() * dayNightCycleSpeed)

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

    // -----------
    // Textures and shaders
    // -----------
    shader = new Framework::Shader(
      RESOURCES_DIR + std::string("shaders/vertex.glsl"),
      RESOURCES_DIR + std::string("shaders/fragment.glsl"),
      true);

    skyboxShader = new Framework::Shader(
      RESOURCES_DIR + std::string("shaders/skyboxVertex.glsl"),
      RESOURCES_DIR + std::string("shaders/skyboxFrag.glsl"),
      true);

    // We only have one shader in the application, so we only bind it here.
    shader->bind();
    Framework::TextureManager::setShader(shader);

#pragma region Textures
    Framework::TextureManager::createTexture(
      "no-texture", glm::vec3(1.0f), { 1, 1 }, 0);
    Framework::TextureManager::createTexture(
      "no-texture-diff", glm::vec3(1.0f), { 1, 1 }, 0);
    Framework::TextureManager::createTexture(
      "no-texture-spec", glm::vec3(0.5f), { 1, 1 }, 1);

    // Load metal plate textures
    Framework::TextureManager::loadTexture(
      "metal_plate_diff",
      TEXTURES_DIR +
        std::string("polyhaven/metal_plate/metal_plate_diff_1k.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "metal_plate_spec",
      TEXTURES_DIR +
        std::string("polyhaven/metal_plate/metal_plate_spec_1k_GIMP.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      1);

    // Coral stone
    Framework::TextureManager::loadTexture(
      "coral_stone_diff",
      TEXTURES_DIR +
        std::string("polyhaven/coral_stone/coral_stone_wall_diff_1k.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "coral_stone_spec",
      TEXTURES_DIR +
        // Roughness is the inverse of spec
        std::string("polyhaven/coral_stone/coral_stone_wall_diff_1k.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      1);

    // Wall
    Framework::TextureManager::loadTexture(
      "wall",
      TEXTURES_DIR +
        std::string("wall.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    getShader()->setInt("u_enableTexture", getEnableTexture());
    getShader()->setInt("u_material.diffuse", 0);
    getShader()->setInt("u_material.specular", 1);

#pragma endregion

    // ------------
    // Map
    // ------------
    map = new Map(shader);
    map->addNewMap("test", RESOURCES_DIR + std::string("/maps/level.map"));
    map->loadMap("test");

    // -------
    // Entities
    // -------

    // Backpack
    backpackModel = Framework::Model(
      RESOURCES_DIR + std::string("models/backpack/backpack.obj"));

    // ---------
    // Rendering
    // ---------
    // Configure camera
    cameraController = Framework::createRef<Framework::CameraController>(
      Framework::CameraType::PERSPECTIVE);
    //    cameraController->setPosition({ 5.0f, -4.0f, 6.0f });
    cameraController->setPosition({ 0.0f, 0.0f, 0.0f });
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

    // Skybox
    auto faces = {
        TEXTURES_DIR + std::string("skybox/demo/right.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/left.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/bottom.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/top.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/front.jpg"),
        TEXTURES_DIR + std::string("skybox/demo/back.jpg"),
    };
    int skyboxId =
      Framework::TextureManager::loadCubemap(skyboxShader,
                                             "skybox_demo",
                                             faces,
                                             Framework::TextureManager::CubeMap,
                                             0);
    skybox = new Framework::Skybox(skyboxId);
    skybox->setScale(700.0f);
    skybox->setPosition({ 0.0f, 0.0f, 0.0f });

    RenderCommand::setClearColor(glm::vec3{ 0.2f, 0.1f, 0.215f });
    shader->setVisualizeMode(RenderCommand::VisualizeMode::NORMAL);
    return true;
}

void
SokobanApplication::run()
{
    while (!glfwWindowShouldClose(getWindow())) {
        RenderCommand::clear();
        glfwPollEvents();
        keyboardInput();

        cameraController->setFarClip(701.0f);
        cameraController->update(*skyboxShader);
        skybox->draw(*skyboxShader);

        cameraController->setFarClip(100.0f);
//        cameraController->update(*shader);
        //        shader->bind();

        //        cameraController->update(*shader);

        // Sun
        sun.draw();

        // Point light
        //        pointLight.draw();
        //        map->getPlayer()->setPosition(pointLight.getPosition());

        // ------
        // Delta time
        // ------
        auto currentFrameTime = (float)glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // --------
        // Camera uploads
        // --------
        map->update();
        map->draw();
        //
        //        backpackModel.setScale(1.0f);
        //        backpackModel.setPosition(
        //          { pointLight.getPosition().x,
        //          pointLight.getPosition().y, 5.0f });
        //        backpackModel.draw(*shader);

        glfwSwapBuffers(getWindow());
    }
}

void
SokobanApplication::shutdown()
{
    DELETE(shader);
    DELETE(map);
    DELETE(skybox);
    DELETE(skyboxShader);

    Framework::TextureManager::clearTextures();
}

/** Keyboard input function. Called every frame */
void
SokobanApplication::keyboardInput()
{
    // ----------
    // Player movement
    // ----------
    Cube* player = (Cube*)map->getPlayer();
    float movementSpeed = 3.0f;
    float moveBy = deltaTime * movementSpeed;

    // Up
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
}

// Source: https://www.glfw.org/docs/3.3/input_guide.html#cursor_pos
void
cursorPos_callback(GLFWwindow* window, double xpos, double ypos)
{
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
          glm::vec2(gApp->getCameraController()->getCameraSpeed());

        gApp->getCameraController()->rotate({ rotation.x, -rotation.y });

        glfwSetCursorPos(
          window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
    }
}

void
mouseScrollBack_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gApp->getCameraController()->moveForward(yoffset);
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

/* Keyboard input function. Used as callback function and is used in OpenGL's
 * input polling system. */
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
    glViewport(0, 0, width, height);

    Framework::ref<Framework::PerspectiveCamera> camera =
      gApp->getCameraController()->getPerspectiveCamera();

    // Match the camera frustum's width and height to the new window size
    Framework::PerspectiveCamera::Frustum frustum = camera->getFrustum();

    frustum.width = static_cast<float>(width);
    frustum.height = static_cast<float>(height);
    camera->setFrustum(frustum);

    // We must compute the projection matrix again for the change to take
    // effect.
    camera->computeProjectionMatrix();

    gApp->setWindowSize({ width, height });

    INFO("Changed window size! Width: {} and height: {}",
         camera->getFrustum().width,
         camera->getFrustum().height);
}
