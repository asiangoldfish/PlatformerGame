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

    // We only have one shader in the application, so we only bind it here.
    shader->bind();
    Framework::TextureManager::setShader(shader);

    Framework::TextureManager::createTexture("no-texture", glm::vec3(1.0f), 0);
    Framework::TextureManager::createTexture(
      "no-texture-diff", glm::vec3(1.0f), 0);
    Framework::TextureManager::createTexture(
      "no-texture-spec", glm::vec3(0.5f), 1);

    Framework::TextureManager::loadTexture(
      "floor",
      TEXTURES_DIR + std::string("floor.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "wall",
      TEXTURES_DIR + std::string("wall.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "player",
      TEXTURES_DIR + std::string("player.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "background",
      TEXTURES_DIR + std::string("destination.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "pillar",
      TEXTURES_DIR + std::string("pillar.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D,
      0);

    Framework::TextureManager::loadTexture(
      "box",
      TEXTURES_DIR + std::string("box.jpg"),
      Framework::TextureManager::TextureFormat::CubeMap,
      0);

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

    getShader()->setInt("u_enableTexture", getEnableTexture());
    getShader()->setInt("u_material.diffuse", 0);
    getShader()->setInt("u_material.specular", 1);

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
    betina = Framework::Model(
      RESOURCES_DIR + std::string("models/betina/betina.obj"));

    // ---------
    // Rendering
    // ---------
    // Configure camera
    camera = std::make_shared<Framework::PerspectiveCamera>(
      Framework::PerspectiveCamera::Frustrum(
        40.0f, getWindowSize().x, getWindowSize().y, 0.01f, 400.0f),
      glm::vec3{ 5, map->getHeight() + 3, 5 });
    //    camera->setLookAtCenter({ worldCenter - 0.5f, 0.0f, worldCenter - 0.5f
    //    });
    camera->rotate({ -90.0f, 0.0f });
    camera->initializeCamera();

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

    RenderCommand::setClearColor(glm::vec3{ 0.1f });

    return true;
}

void
SokobanApplication::run()
{
    while (!glfwWindowShouldClose(getWindow())) {
        RenderCommand::clear();
        glfwPollEvents();

        keyboardInput();

        // ---------
        // Camera
        // ---------
        shader->setFloat3("u_cameraPosition", camera->getPosition());

        // Sun
        sun.draw();

        // Point light
        pointLight.draw();
        map->getPlayer()->setPosition(pointLight.getPosition());

        backpackModel.setPosition(
          { pointLight.getPosition().x, pointLight.getPosition().y, 5.0f });
        backpackModel.draw(*shader);

        betina.setPosition({ pointLight.getPosition().x,
                                    pointLight.getPosition().y - 4.0f,
                                    10.0f });
        betina.draw(*shader);



        // ------
        // Delta time
        // ------
        auto currentFrameTime = (float)glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        // --------
        // Camera uploads
        // --------
        shader->setMat4("u_projection", camera->getProjectionMatrix());
        shader->setMat4("u_view", camera->getViewMatrix());

        map->update();
        map->draw();

        glfwSwapBuffers(getWindow());
    }
}

void
SokobanApplication::shutdown()
{
    for (auto& wall : walls) {
        delete wall;
        wall = nullptr;
    }

    for (auto& box : boxes) {
        delete box;
        box = nullptr;
    }

    for (auto& pillar : pillars) {
        delete pillar;
        pillar = nullptr;
    }

    for (auto& destination : destinations) {
        delete destination;
        destination = nullptr;
    }

    delete shader;
    shader = nullptr;

    delete map;
    map = nullptr;

    Framework::TextureManager::clearTextures();
}

/** Keyboard input function. Called every frame */
void
SokobanApplication::keyboardInput()
{
    // -------
    // Move camera
    // -------

    //    // Counterclockwise rotation
    //    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    //        //        gApp->rotateCamera(false);
    //        gApp->getCamera()->moveSideway(-1);
    //    }
    //
    //    // Clockwise rotation
    //    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    //        //        gApp->rotateCamera(true);
    //        gApp->getCamera()->moveSideway(1);
    //    }
    //
    //    float fovStep = 30.0f * gApp->getDeltaTime();
    //
    //    // Zoom in
    //    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
    //        //        Framework::PerspectiveCamera::Frustrum frustrum =
    //        //          gApp->getCamera()->getFrustrum();
    //        //        frustrum.angle =
    //        //          std::max(gApp->getCamera()->getFrustrum().angle -
    //        fovStep,
    //        //          05.0f);
    //        //        gApp->getCamera()->setFrustrum(frustrum);
    //
    //        gApp->getCamera()->moveUp(1.0f);
    //    }
    //
    //    // Zoom out
    //    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    //        //        Framework::PerspectiveCamera::Frustrum frustrum =
    //        //          gApp->getCamera()->getFrustrum();
    //        //        frustrum.angle =
    //        //          std::min(gApp->getCamera()->getFrustrum().angle +
    //        fovStep,
    //        //          120.0f);
    //        //        gApp->getCamera()->setFrustrum(frustrum);
    //
    //        gApp->getCamera()->moveUp(-1.0f);
    //    }

    // ----------
    // Player movement
    // ----------
    Cube* player = (Cube*)map->getPlayer();
    float movementSpeed = 3.0f;
    float moveBy = deltaTime * movementSpeed;

    // Up
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        //        player->move({ 0.0f, moveBy, 0.0f });
        camera->moveForward(moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        //        player->move({ 0.0f, -moveBy, 0.0f });
        camera->moveForward(-moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        //        player->move({ moveBy, 0.0f, 0.0f });
        camera->moveSideway(moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        //        player->move({ -moveBy, 0.0f, 0.0f });
        camera->moveSideway(-moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_Q) == GLFW_PRESS) {
        camera->moveUp(moveBy);
    }
    if (glfwGetKey(getWindow(), GLFW_KEY_E) == GLFW_PRESS) {
        camera->moveUp(-moveBy);
    }
}

void
SokobanApplication::movePlayer(glm::vec3 direction)
{
    Framework::Entity* player = map->getPlayer();
    glm::vec3 playerPos = player->getPosition();
    bool canMove = true;

    //    if (canMove) { // Pillars
    for (const auto& pillar : pillars) {
        if (pillar->getPosition().x == playerPos.x + direction.x &&
            pillar->getPosition().z == playerPos.z + direction.z) {

            canMove = false;
            break;
        }
    }
    //    }

    // Only move the player if nothing blocked the way
    //    if (canMove) {
    if (moveBox(direction)) {
        player->move(direction);
    }
    //    }
}

/**
 * Move a box that the player is facing and is next to.
 * @param direction Direction in which the player is moving in
 * @return true if there is no boxes in the way or the box can move. False if
 * not
 */
bool
SokobanApplication::moveBox(glm::vec3 direction)
{
    glm::vec3 playerNewPos = map->getPlayer()->getPosition() + direction;

    bool canMove = true;
    Cube* adjacentBox = nullptr;

    // Figure out if the player is pushing a box. To do this, we check whether
    // the player's position plus direction has an entity.
    for (const auto& box : boxes) { // Box
        // Check whether the player is adjacent to this box
        if (playerNewPos.x == box->getPosition().x &&
            playerNewPos.z == box->getPosition().z) {
            adjacentBox = box;
            break;
        }
    }

    // If there is no adjacent box, then return true
    if (!adjacentBox) {
        return true;
    }

    bool newPositionFound =
      true; // If an empty position is found, then this is true

    // There is an adjacent box in the direction that the player faces. We now
    // check if we can move this box. We do this by checking whether there is an
    // adjacent wall or pillar in the direction we will move in. Check for walls
    glm::vec3 boxNewPosition = adjacentBox->getPosition() + direction;
    // Walls are positioned with x equals 0 or 9, and the same for z.
    newPositionFound = !(boxNewPosition.x == 0 || boxNewPosition.x == 9 ||
                         boxNewPosition.z == 0 || boxNewPosition.z == 9);

    if (newPositionFound) {
        for (const auto& pillar : gApp->getPillars()) { // Pillar
            if (boxNewPosition.x == pillar->getPosition().x &&
                boxNewPosition.z == pillar->getPosition().z) {
                newPositionFound = false; // A pillar was found
                break;
            }
        }
    }

    // Check for other boxes
    if (newPositionFound) {
        for (const auto& box : gApp->getBoxes()) { // Boxes
            if (adjacentBox !=
                box) { // Avoid checking against the *adjacentBox* itself
                if (boxNewPosition.x == box->getPosition().x &&
                    boxNewPosition.z == box->getPosition().z) {
                    newPositionFound = false; // A pillar was found
                    break;
                }
            }
        }
    }

    // If no walls, other boxes or pillars are in the way, then return true.
    // Else return false
    if (newPositionFound) {
        adjacentBox->move(direction);

        // If overlapping a destination, change color to dark blue
        bool foundDestination = false;
        for (const auto& destination : gApp->getDestinations()) { // Pillar
            if (boxNewPosition.x == destination->getPosition().x &&
                boxNewPosition.z == destination->getPosition().z) {
                foundDestination = true;
                break;
            }
        }

        foundDestination
          ? adjacentBox->setColor({ 0.0f, 0.0f, 1.0f, 1.0f })
          : adjacentBox->setColor(adjacentBox->getOriginalColor());
        return true;
    } else {
        return false;
    }
}

void
SokobanApplication::rotateCamera(bool rotateRight)
{
    // Create a "clock" that circulates between 0 and 360 time units
    if (degrees >= 360.0f) {
        degreesDirection = -1;
    } else if (degrees <= 0) {
        degreesDirection = 1;
    }

    float rotationSpeed = 45.0f;
    degrees += rotationSpeed * getDeltaTime() * (rotateRight ? 1.0f : -1.0f);
    const float distance = getCameraDistance();

    float xPos = worldCenter + glm::cos(glm::radians(degrees)) * distance;
    float zPos = worldCenter + glm::sin(glm::radians(degrees)) * distance;

    getCamera()->setPosition({ xPos, getCamera()->getPosition().y, zPos });
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
        glm::vec2 rotation = { xpos - gApp->getWindowSize().x / 2,
                               ypos - gApp->getWindowSize().y / 2 };

        gApp->getCamera()->rotate(rotation * gApp->getCameraSpeed());

        glfwSetCursorPos(
          window, gApp->getWindowSize().x / 2, gApp->getWindowSize().y / 2);
    }
}

void
mouseScrollBack_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    gApp->getCamera()->moveForward(yoffset);
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

        // Toggle color for all entities except floor
    }
}

void
framebufferSize_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);

    auto camera = gApp->getCamera();

    // Match the camera frustrum's width and height to the new window size
    Framework::PerspectiveCamera::Frustrum frustrum = camera->getFrustrum();

    frustrum.width = width;
    frustrum.height = height;
    camera->setFrustrum(frustrum);

    // We must compute the projection matrix again for the change to take
    // effect.
    camera->computeProjectionMatrix();

    INFO("Changed window size! Width: {} and height: {}",
         gApp->getCamera()->getFrustrum().width,
         gApp->getCamera()->getFrustrum().height);
}
