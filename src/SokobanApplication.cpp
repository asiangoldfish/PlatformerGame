/**
 * Copyright 2023 Khai Duong
 */

#include "SokobanApplication.h"

// Framework API
#include "GeometricTools.h"
#include "PerspectiveCamera.h"
#include "RenderCommands.h"
#include "Shader.h"
#include "Input.h"

// App Components
#include "Cube.h"
#include "Floor.h"
#include "Map.h"
#include "gameMath.h"

// Standard libraries
#include <algorithm>
#include <string>

#define TIMEOFDAY ((float)glfwGetTime() * dayNightCycleSpeed)

SokobanApplication* gApp = nullptr;

// Function prototypes
void
cursorPos_callback(GLFWwindow* window, double xpos, double ypos);
void
cursorMouseButton_callback(GLFWwindow* window, int, int, int);
static void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void
spawnWall(glm::vec3 newPos, glm::vec3 newColor);
void
spawnPillar(Cube& targetPillar);
static void
spawnDestinations(Cube& destination);
static void
spawnBoxes(Cube& box);
static void
spawnPlayer(Cube& player);

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

    Framework::TextureManager::createTexture("no-texture");

    Framework::TextureManager::loadTexture(
      "floor",
      TEXTURES_DIR + std::string("floor.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

    Framework::TextureManager::loadTexture(
      "wall",
      TEXTURES_DIR + std::string("wall.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

    Framework::TextureManager::loadTexture(
      "player",
      TEXTURES_DIR + std::string("player.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

    Framework::TextureManager::loadTexture(
      "background",
      TEXTURES_DIR + std::string("destination.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

    Framework::TextureManager::loadTexture(
      "pillar",
      TEXTURES_DIR + std::string("pillar.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

    Framework::TextureManager::loadTexture(
      "box",
      TEXTURES_DIR + std::string("box.jpg"),
      Framework::TextureManager::TextureFormat::CubeMap);

    getShader()->setInt("u_enableTexture", getEnableTexture());

    // ------------
    // Map
    // ------------
    map = new Map(shader);
    map->addNewMap("test", RESOURCES_DIR + std::string("/maps/level.map"));
    map->loadMap("test");

    // -------
    // Entities
    // -------
    // Player
    player = new Cube(gApp->getShader());
    player->setTextureName("player");

    player->setScale(0.8f);
    player->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    player->setPosition({ 3.0f, 0.4f, 4.0f });

    // ---------
    // Rendering
    // ---------
    // Configure camera
    camera = std::make_shared<Framework::PerspectiveCamera>(
      Framework::PerspectiveCamera::Frustrum(
        40.0f, getWindowSize().x, getWindowSize().y, 0.01f, 400.0f),
      glm::vec3{ 5, map->getHeight() + 3, 20 });
    //    camera->setLookAtCenter({ worldCenter - 0.5f, 0.0f, worldCenter - 0.5f
    //    });
    camera->rotate({ -90.0f, 0.0f });
    camera->initializeCamera();

    // Screen
    RenderCommand::setClearColor(glm::vec3(0.5f, 0.5f, 0.5f));

    // -------------
    // Lighting
    // -------------
    light.setEnableLighting(true);
    light.setShader(shader);

    // Sun
    sun = new Cube(gApp->getShader());
    sun->setTextureName("no-texture");
    sun->setPosition(
      { mapSize / 2.0f, 0.0f, sunDistance });  // Start in the morning
    sun->setColor({ 0.7f, 0.4f, 0.0f, 1.0f }); // Orange color

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

        // Sun
        drawSun();

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

        // Player
        player->draw();

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

    delete player;
    player = nullptr;

    delete sun;
    sun = nullptr;

    delete map;
    map = nullptr;

    Framework::TextureManager::clearTextures();
}

void
SokobanApplication::drawSun()
{

    // Move the sun across the sky
    sun->setPosition({ std::cos(lerp(TIMEOFDAY)) * sunDistance,
                       std::sin(lerp(TIMEOFDAY)) * sunDistance,
                       sun->getPosition().z });

    if (!shader) {
        return;
    }

    // Ambient color
    auto ambientColor =
      glm::vec3(1.0f) -
      glm::vec3(std::cos(TIMEOFDAY) * 0.1f,
                (std::cos(TIMEOFDAY) + std::sin(TIMEOFDAY)) * 0.1f,
                std::sin(TIMEOFDAY * 2) * 0.25f);

    // The color temperature is colder in the evening and morning, and warmer
    // during the day
    float timeFactor = (std::sin(TIMEOFDAY) + 1.0f) * 0.5f;
    ambientColor *= timeFactor;
    shader->setFloat3("u_ambientColor", ambientColor);
    RenderCommand::setClearColor(ambientColor);

    // Camera position for specularity
    shader->setFloat3("u_cameraPosition", camera->getPosition());

    // Light
    light.setPosition(sun->getPosition());
    light.setEnableLighting(true);
    light.setSpecularColor(glm::vec3(std::cos(TIMEOFDAY)));
    light.setDiffuseColor(glm::vec3(std::cos(TIMEOFDAY)));
    light.update();

    // Disable lighting effects on the sun object only
    Framework::TextureManager::bind("no-texture");
    shader->setInt("u_enableLighting", 0);
    sun->draw();
    shader->setInt("u_enableLighting", 1);
}

/** Keyboard input function. Called every frame */
void
SokobanApplication::keyboardInput()
{
    // -------
    // Move camera
    // -------

    // Counterclockwise rotation
    if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
        //        gApp->rotateCamera(false);
        gApp->getCamera()->moveSideway(-1);
    }

    // Clockwise rotation
    if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
        //        gApp->rotateCamera(true);
        gApp->getCamera()->moveSideway(1);
    }

    float fovStep = 30.0f * gApp->getDeltaTime();

    // Zoom in
    if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
        //        Framework::PerspectiveCamera::Frustrum frustrum =
        //          gApp->getCamera()->getFrustrum();
        //        frustrum.angle =
        //          std::max(gApp->getCamera()->getFrustrum().angle - fovStep,
        //          05.0f);
        //        gApp->getCamera()->setFrustrum(frustrum);

        gApp->getCamera()->moveUp(1.0f);
    }

    // Zoom out
    if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
        //        Framework::PerspectiveCamera::Frustrum frustrum =
        //          gApp->getCamera()->getFrustrum();
        //        frustrum.angle =
        //          std::min(gApp->getCamera()->getFrustrum().angle + fovStep,
        //          120.0f);
        //        gApp->getCamera()->setFrustrum(frustrum);

        gApp->getCamera()->moveUp(-1.0f);
    }

    shader->setFloat3("u_cameraPosition", camera->getPosition());
}

void
SokobanApplication::movePlayer(glm::vec3 direction)
{
    glm::vec3 playerPos = player->getPosition();
    bool canMove = true;

    // Only move the player if there is no wall or pillar in the way
    canMove = !(playerPos.x + direction.x == 0 ||
                playerPos.x + direction.x == mapSize - 1 ||
                playerPos.z + direction.z == 0 ||
                playerPos.z + direction.z == mapSize - 1);

    if (canMove) { // Pillars
        for (const auto& pillar : pillars) {
            if (pillar->getPosition().x == playerPos.x + direction.x &&
                pillar->getPosition().z == playerPos.z + direction.z) {

                canMove = false;
                break;
            }
        }
    }

    // Only move the player if nothing blocked the way
    if (canMove) {
        if (moveBox(direction)) {
            getPlayer()->move(direction);
        }
    }
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
    glm::vec3 playerNewPos = player->getPosition() + direction;

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
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
    } else {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }
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
    }
}

/* Keyboard input function. Used as callback function and is used in OpenGL's
 * input polling system. */
void
keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    // Quit application
    if (key == GLFW_KEY_Q && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, 1);
    }

    // Toggle textures
    if (key == GLFW_KEY_T && action == GLFW_PRESS) {
        gApp->setEnableTexture(!gApp->getEnableTexture());
        gApp->getShader()->setInt("u_enableTexture", gApp->getEnableTexture());

        // Toggle color for all entities except floor
    }

    // ----------
    // Player movement
    // ----------
    // Player can only move once per key press
    struct MoveDirection
    {
        bool up = false, down = false, left = false, right = false;
    };
    static MoveDirection direction;

    // Up
    if (key == GLFW_KEY_UP && action == GLFW_PRESS && !direction.up) {
        direction.up = true;
        gApp->movePlayer({ 1.0f, 0.0f, 0.0f });
    }
    if ((key == GLFW_KEY_UP) && action == GLFW_RELEASE) {
        direction.up = false;
    }

    // Down
    if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && !direction.down) {
        direction.down = true;
        gApp->movePlayer({ -1.0f, 0.0f, 0.0f });
    }
    if ((key == GLFW_KEY_DOWN) && action == GLFW_RELEASE) {
        direction.down = false;
    }

    // Left
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && !direction.left) {
        direction.left = true;
        gApp->movePlayer({ 0.0f, 0.0f, -1.0f });
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
        direction.left = false;
    }

    // Right
    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && !direction.right) {
        direction.right = true;
        gApp->movePlayer({ 0.0f, 0.0f, 1.0f });
    }
    if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
        direction.right = false;
    }
}

void
spawnWall(glm::vec3 newPos, glm::vec3 newColor)
{
    Cube* newWall = new Cube(gApp->getShader());

    newWall->setScale(1.0f);
    newWall->setColor(glm::vec4(newColor, 1.0f));
    newWall->setPosition(newPos);
    gApp->getWalls().push_back(newWall);
}

/**
 * Spawn a pillar, but ensure that it does not collide with walls or other
 * pillars
 * @param targetPillar The selected pillar to spawn
 */
void
spawnPillar(Cube& targetPillar)
{
    glm::vec3 newPos = glm::vec3(std::round(rng(1, mapSize - 2)),
                                 targetPillar.getPosition().y,
                                 std::round(rng(1, mapSize - 2)));

    bool newPositionFound =
      true; // If an empty position is found, then this is true
    for (const auto& pillar : gApp->getPillars()) {
        if (&targetPillar != pillar) { // Ensure they are not the same pillars
            if (newPos.x == pillar->getPosition().x &&
                newPos.z == pillar->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    if (!newPositionFound) {
        // Recursively check for new position again
        spawnPillar(targetPillar);
    } else {
        // Set new position
        targetPillar.setPosition(newPos);
    }
}

/**
 * Create destinations
 * They must not conflict with the positions of walls and pillars
 * @param n Max destinations to spawn
 * @param currentIndex The current destination index. This is used for
 * recursively finding a new available position.
 */
void
spawnDestinations(Cube& destination)
{
    glm::vec3 newPos = glm::vec3(std::round(rng(1, mapSize - 1)),
                                 destination.getPosition().y,
                                 std::round(rng(1, mapSize - 1)));

    bool newPositionFound = true;
    for (const auto& targetDest : gApp->getDestinations()) {
        if (&destination != targetDest) {
            // Ensure they are not the same entity
            if (newPos.x == targetDest->getPosition().x &&
                newPos.z == targetDest->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    // Check collision with pillars
    if (newPositionFound) {
        for (const auto& pillar : gApp->getPillars()) {
            if (newPos.x == pillar->getPosition().x &&
                newPos.z == pillar->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    if (!newPositionFound) {
        // Recursively check for new position again
        spawnDestinations(destination);
    } else { // Set new position
        destination.setPosition(newPos);
    }
}

/**
 * Spawn player
 * They must not conflict with anything's positions
 * @param currentIndex The current destination index. This is used for
 * recursively finding a new available position.
 */
void
spawnBoxes(Cube& box)
{
    glm::vec3 newPos = glm::vec3(std::round(rng(1, mapSize - 1)),
                                 box.getScale().y / 2.0f,
                                 std::round(rng(1, mapSize - 1)));

    // Ensure it does not overlap other walls
    bool newPositionFound = true;

    // Do not collision with self
    for (const auto& targetBox : gApp->getBoxes()) {
        if (&box != targetBox) {
            if (newPos.x == targetBox->getPosition().x &&
                newPos.z == targetBox->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    // Check collision with pillars
    if (newPositionFound) {
        for (const auto& pillar : gApp->getPillars()) {
            if (newPos.x == pillar->getPosition().x &&
                newPos.z == pillar->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    // Check collision with destinations
    if (newPositionFound) {
        for (const auto& destination : gApp->getDestinations()) {
            if (newPos.x == destination->getPosition().x &&
                newPos.z == destination->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    if (!newPositionFound) { // Recursively check for new position again
        spawnBoxes(box);
    } else { // Set new position
        box.setPosition(newPos);
    }
}

/**
 * Create box
 * They must not conflict with the positions of walls and pillars
 * @param currentIndex The current destination index. This is used for
 * recursively finding a new available position.
 */
void
spawnPlayer(Cube& player)
{
#define RANDOM_POSITION std::round(rng(1, mapSize - 1))
    glm::vec3 newPos =
      glm::vec3(RANDOM_POSITION, player.getPosition().y, RANDOM_POSITION);

    // Sometimes the rng fails. Clamp the values if that happens.
    newPos.x = std::clamp(newPos.x, 1.0f, 8.0f);
    newPos.z = std::clamp(newPos.z, 1.0f, 8.0f);

    // Ensure it does not overlap other walls
    bool newPositionFound =
      true; // If an empty position is found, then this is true

    // Check collision with pillars
    for (const auto& pillar : gApp->getPillars()) {
        if (newPos.x == pillar->getPosition().x &&
            newPos.z == pillar->getPosition().z) {
            newPositionFound = false;
            break;
        }
    }

    // Check collision with boxes
    if (newPositionFound) {
        for (const auto& box : gApp->getBoxes()) {
            if (newPos.x == box->getPosition().x &&
                newPos.z == box->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    // Check collision with destinations
    if (newPositionFound) {
        for (const auto& destination : gApp->getDestinations()) {
            if (newPos.x == destination->getPosition().x &&
                newPos.z == destination->getPosition().z) {
                newPositionFound = false;
                break;
            }
        }
    }

    if (!newPositionFound) { // Recursively check for new position again
        spawnPlayer(player);
    } else { // Set new position
        player.setPosition(newPos);
    }
}
