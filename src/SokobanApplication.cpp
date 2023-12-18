/**
 * Copyright 2023 Khai Duong
 */

#include "SokobanApplication.h"

// Framework API
#include "GeometricTools.h"
#include "PerspectiveCamera.h"
#include "RenderCommands.h"
#include "Shader.h"

// App Components
#include "Cube.h"
#include "Floor.h"
#include "gameMath.h"

// Standard libraries
#include <algorithm>

#define TIMEOFDAY ((float)glfwGetTime() * dayNightCycleSpeed)

SokobanApplication *gApp = nullptr;

/** Keyboard input function. Called every frame */
void SokobanApplication::keyboardInput() {
  // -------
  // Move camera
  // -------

  // Counterclockwise rotation
  if (glfwGetKey(getWindow(), GLFW_KEY_A) == GLFW_PRESS) {
    gApp->rotateCamera(false);
  }

  // Clockwise rotation
  if (glfwGetKey(getWindow(), GLFW_KEY_D) == GLFW_PRESS) {
    gApp->rotateCamera(true);
  }

  float fovStep = 30.0f * gApp->getDeltaTime();

  // Zoom in
  if (glfwGetKey(getWindow(), GLFW_KEY_W) == GLFW_PRESS) {
    Framework::PerspectiveCamera::Frustrum frustrum =
        gApp->getCamera()->getFrustrum();
    frustrum.angle =
        std::max(gApp->getCamera()->getFrustrum().angle - fovStep, 05.0f);
    gApp->getCamera()->setFrustrum(frustrum);
  }

  // Zoom out
  if (glfwGetKey(getWindow(), GLFW_KEY_S) == GLFW_PRESS) {
    Framework::PerspectiveCamera::Frustrum frustrum =
        gApp->getCamera()->getFrustrum();
    frustrum.angle =
        std::min(gApp->getCamera()->getFrustrum().angle + fovStep, 120.0f);
    gApp->getCamera()->setFrustrum(frustrum);
  }

  shader->setFloat3("u_cameraPosition", camera->getPosition());
}

/* Keyboard input function. Used as callback function and is used in OpenGL's
 * input polling system. */
static void keyCallback(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
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
  struct MoveDirection {
    bool up = false, down = false, left = false, right = false;
  };
  static MoveDirection direction;

  // Up
  if (key == GLFW_KEY_UP && action == GLFW_PRESS && !direction.up) {
    direction.up = true;
    gApp->movePlayer({1.0f, 0.0f, 0.0f});
  }
  if ((key == GLFW_KEY_UP) && action == GLFW_RELEASE) {
    direction.up = false;
  }

  // Down
  if (key == GLFW_KEY_DOWN && action == GLFW_PRESS && !direction.down) {
    direction.down = true;
    gApp->movePlayer({-1.0f, 0.0f, 0.0f});
  }
  if ((key == GLFW_KEY_DOWN) && action == GLFW_RELEASE) {
    direction.down = false;
  }

  // Left
  if (key == GLFW_KEY_LEFT && action == GLFW_PRESS && !direction.left) {
    direction.left = true;
    gApp->movePlayer({0.0f, 0.0f, -1.0f});
  }
  if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE) {
    direction.left = false;
  }

  // Right
  if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS && !direction.right) {
    direction.right = true;
    gApp->movePlayer({0.0f, 0.0f, 1.0f});
  }
  if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE) {
    direction.right = false;
  }
}

static void spawnWall(glm::vec3 newPos, glm::vec3 newColor) {
  Cube *newWall =
      new Cube(gApp->getShader(), GeometricTools::UnitCubeGeometry3D(),
               GeometricTools::UnitCubeGeometry3DIndices());

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
static void spawnPillar(Cube &targetPillar) {
  glm::vec3 newPos =
      glm::vec3(std::round(rng(1, mapSize - 2)), targetPillar.getPosition().y,
                std::round(rng(1, mapSize - 2)));

  bool newPositionFound =
      true; // If an empty position is found, then this is true
  for (const auto &pillar : gApp->getPillars()) {
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
static void spawnDestinations(Cube &destination) {
  glm::vec3 newPos =
      glm::vec3(std::round(rng(1, mapSize - 1)), destination.getPosition().y,
                std::round(rng(1, mapSize - 1)));

  bool newPositionFound = true;
  for (const auto &targetDest : gApp->getDestinations()) {
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
    for (const auto &pillar : gApp->getPillars()) {
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
static void spawnBoxes(Cube &box) {
  glm::vec3 newPos =
      glm::vec3(std::round(rng(1, mapSize - 1)), box.getScale().y / 2.0f,
                std::round(rng(1, mapSize - 1)));

  // Ensure it does not overlap other walls
  bool newPositionFound = true;

  // Do not collision with self
  for (const auto &targetBox : gApp->getBoxes()) {
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
    for (const auto &pillar : gApp->getPillars()) {
      if (newPos.x == pillar->getPosition().x &&
          newPos.z == pillar->getPosition().z) {
        newPositionFound = false;
        break;
      }
    }
  }

  // Check collision with destinations
  if (newPositionFound) {
    for (const auto &destination : gApp->getDestinations()) {
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
static void spawnPlayer(Cube &player) {
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
  for (const auto &pillar : gApp->getPillars()) {
    if (newPos.x == pillar->getPosition().x &&
        newPos.z == pillar->getPosition().z) {
      newPositionFound = false;
      break;
    }
  }

  // Check collision with boxes
  if (newPositionFound) {
    for (const auto &box : gApp->getBoxes()) {
      if (newPos.x == box->getPosition().x &&
          newPos.z == box->getPosition().z) {
        newPositionFound = false;
        break;
      }
    }
  }

  // Check collision with destinations
  if (newPositionFound) {
    for (const auto &destination : gApp->getDestinations()) {
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

void SokobanApplication::movePlayer(glm::vec3 direction) {
  glm::vec3 playerPos = player->getPosition();
  bool canMove = true;

  // Only move the player if there is no wall or pillar in the way
  canMove = !(playerPos.x + direction.x == 0 ||
              playerPos.x + direction.x == mapSize - 1 ||
              playerPos.z + direction.z == 0 ||
              playerPos.z + direction.z == mapSize - 1);

  if (canMove) { // Pillars
    for (const auto &pillar : pillars) {
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
bool SokobanApplication::moveBox(glm::vec3 direction) {
  glm::vec3 playerNewPos = player->getPosition() + direction;

  bool canMove = true;
  Cube *adjacentBox = nullptr;

  // Figure out if the player is pushing a box. To do this, we check whether the
  // player's position plus direction has an entity.
  for (const auto &box : boxes) { // Box
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
    for (const auto &pillar : gApp->getPillars()) { // Pillar
      if (boxNewPosition.x == pillar->getPosition().x &&
          boxNewPosition.z == pillar->getPosition().z) {
        newPositionFound = false; // A pillar was found
        break;
      }
    }
  }

  // Check for other boxes
  if (newPositionFound) {
    for (const auto &box : gApp->getBoxes()) { // Boxes
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

  // If no walls, other boxes or pillars are in the way, then return true. Else
  // return false
  if (newPositionFound) {
    adjacentBox->move(direction);

    // If overlapping a destination, change color to dark blue
    bool foundDestination = false;
    for (const auto &destination : gApp->getDestinations()) { // Pillar
      if (boxNewPosition.x == destination->getPosition().x &&
          boxNewPosition.z == destination->getPosition().z) {
        foundDestination = true;
        break;
      }
    }

    foundDestination ? adjacentBox->setColor({0.0f, 0.0f, 1.0f, 1.0f})
                     : adjacentBox->setColor(adjacentBox->getOriginalColor());
    return true;
  } else {
    return false;
  }
}

void SokobanApplication::rotateCamera(bool rotateRight) {
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

  getCamera()->setPosition({xPos, getCamera()->getPosition().y, zPos});
}

bool SokobanApplication::init() {
  // ------
  // Configure application
  // ------
  if (!GLFWApplication::init()) {
    return false;
  }

  gApp = this; // This needs to happen before anything else to prevent segfault
  glfwSetKeyCallback(getWindow(), keyCallback);

  // -----------
  // Textures and shaders
  // -----------
  shader = new Framework::Shader(
      RESOURCES_DIR + std::string("shaders/vertex.glsl"),
      RESOURCES_DIR + std::string("shaders/fragment.glsl"), true);

  // We only have one shader in the application, so we only bind it here.
  shader->bind();
  textureManager.setShader(shader);

  textureManager.createTexture("no-texture");

  textureManager.loadTexture(
      "floor", TEXTURES_DIR + std::string("floor.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

  textureManager.loadTexture(
      "wall", TEXTURES_DIR + std::string("wall.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

  textureManager.loadTexture(
      "player", TEXTURES_DIR + std::string("player.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

  textureManager.loadTexture(
      "pillar", TEXTURES_DIR + std::string("pillar.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

  textureManager.loadTexture(
      "destination", TEXTURES_DIR + std::string("destination.jpg"),
      Framework::TextureManager::TextureFormat::Texture2D);

  textureManager.loadTexture("box", TEXTURES_DIR + std::string("box.jpg"),
                             Framework::TextureManager::TextureFormat::CubeMap);

  getShader()->setInt("u_enableTexture", getEnableTexture());

  // -------
  // Entities
  // -------
  // Player
  player = new Cube(gApp->getShader(), GeometricTools::UnitCubeGeometry3D(),
                    GeometricTools::UnitCubeGeometry3DIndices());

  player->setScale(0.8f);
  player->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
  player->setPosition({3.0f, 0.4f, 4.0f});

  // Floor
  floor = new Floor(
      shader,
      GeometricTools::UnitGridGeometry2D(mapSize, mapSize, tileSize, tileSize),
      GeometricTools::UnitGridIndices2D(mapSize * mapSize), tileSize);
  floor->rotate(-90.0f, 0.0f, -90.0f);
  floor->move({0.0f, -0.001f, 0.0f}); // Nudged down to mitigate Z-fighting.
  floor->setTiles(mapSize, mapSize);
  floor->setScale({tileSize, tileSize, 0.0f});

  /* Generate and configure walls
   * First and last row should have walls on all tiles. All rows in between
   * should have one wall in the beginning and one at the end.
   * XXXXXXXXXXXXXXX
   * x             x
   * x             x
   * x             x
   * XXXXXXXXXXXXXXX
   */
  // Generate first row of walls
  const float cubeHeight = 0.5f;
  float wallColor = 0.4f;
  for (int i = 0; i < mapSize; i++) {
    spawnWall({0.0f, cubeHeight, i}, glm::vec3{wallColor});
  }

  // Generate last row of walls
  for (int i = 0; i < mapSize; i++) {
    spawnWall({mapSize - 1.0f, cubeHeight, i}, glm::vec3{wallColor});
  }

  // Generate walls in between first and last row
  for (int i = 1; i < mapSize - 1; i++) {
    spawnWall({i, cubeHeight, 0.0f}, glm::vec3{wallColor});
    spawnWall({i, cubeHeight, mapSize - 1.0f}, glm::vec3{wallColor});
  }

  // Pillars
  for (int i = 0; i < maxPillars; i++) {
    Cube *newPillar =
        new Cube(gApp->getShader(), GeometricTools::UnitCubeGeometry3D(),
                 GeometricTools::UnitCubeGeometry3DIndices());

    newPillar->setScale({0.6f, 1.5f, 0.6f});
    newPillar->move({0.0f, newPillar->getScale().y / 2.0f, 0.0f});
    newPillar->setColor(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));

    pillars.push_back(newPillar);
  }

  // Destinations
  for (int i = 0; i < maxDestinations; i++) {
    Cube *newDestination =
        new Cube(gApp->getShader(), GeometricTools::UnitCubeGeometry3D(),
                 GeometricTools::UnitCubeGeometry3DIndices());

    newDestination->setScale({0.8f, 0.1f, 0.8f});
    newDestination->move({0.0f, newDestination->getScale().y / 2.0f, 0.0f});
    newDestination->setColor(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f));

    destinations.push_back(newDestination);
  }

  // Boxes
  for (int i = 0; i < maxBoxes; i++) {
    Cube *newBox =
        new Cube(gApp->getShader(), GeometricTools::UnitCubeGeometry3D(),
                 GeometricTools::UnitCubeGeometry3DIndices());

    newBox->setScale({0.8f, 0.8f, 0.8f});
    newBox->move({0.0f, newBox->getScale().y / 2.0f, 0.0f});
    newBox->setColor(glm::vec4(0.5f, 0.75f, 0.3f, 1.0f));
    newBox->setOriginalColor(newBox->getColor());

    boxes.push_back(newBox);
  }

  // ---------
  // Rendering
  // ---------
  // Configure camera
  camera = std::make_shared<Framework::PerspectiveCamera>(
      Framework::PerspectiveCamera::Frustrum(40.0f, 1024.0f, 1024.0f, 0.01f,
                                             400.0f),
      glm::vec3{worldCenter + glm::cos(glm::radians(degrees)) * cameraDistance,
                10.0f,
                worldCenter + glm::sin(glm::radians(degrees)) * cameraDistance}

  );
  camera->setPitch(-30);
  camera->setEnablePanning(false);
  camera->setLookAtCenter({worldCenter - 0.5f, 0.0f, worldCenter - 0.5f});
  camera->initializeCamera();

  // Screen
  RenderCommand::setClearColor(glm::vec3(0.5f, 0.5f, 0.5f));

  // Spawn pillars. Walls are on the edge of the map, so we spawn anywhere
  // inside it. Spawn the first pillar
  if (!pillars.empty()) {
    for (auto &pillar : pillars) {
      spawnPillar(*pillar);
    }
  }

  // Spawn destinations and not overlap with walls and pillars
  for (auto &destination : destinations) {
    spawnDestinations(*destination);
  }

  // Spawn boxes and not overlap with walls or destinations and pillars
  for (auto &box : boxes) {
    spawnBoxes(*box);
  }

  // Spawn player and not overlap with anything
  spawnPlayer(*player);

  // -------------
  // Lighting
  // -------------
  light.setEnableLighting(true);
  light.setShader(shader);

  // Sun
  sun = new Cube(gApp->getShader(), GeometricTools::UnitCubeGeometry3D(),
                 GeometricTools::UnitCubeGeometry3DIndices());
  sun->setPosition({mapSize / 2.0f, 0.0f, sunDistance}); // Start in the morning
  sun->setColor({0.7f, 0.4f, 0.0f, 1.0f});               // Orange color

  RenderCommand::setClearColor(glm::vec3{0.1f});

  return true;
}

void SokobanApplication::run() {
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

    // --------
    // Draw entities
    // --------
    // Floor
    textureManager.bind("floor");
    floor->draw();

    // Wall
    textureManager.bind("wall");
    for (const auto &wall : walls) {
      wall->draw();
    }

    // Pillars
    textureManager.bind("pillar");
    for (const auto &pillar : pillars) {
      pillar->draw();
    }

    // Destinations
    textureManager.bind("destination");
    for (const auto &destination : destinations) {
      destination->draw();
    }

    // Boxes
    textureManager.bind("box");
    for (const auto &box : boxes) {
      box->draw();
    }

    // Player
    textureManager.bind("player");
    player->draw();

    glfwSwapBuffers(getWindow());
  }
}

void SokobanApplication::shutdown() {
  for (auto &wall : walls) {
    delete wall;
    wall = nullptr;
  }

  for (auto &box : boxes) {
    delete box;
    box = nullptr;
  }

  for (auto &pillar : pillars) {
    delete pillar;
    pillar = nullptr;
  }

  for (auto &destination : destinations) {
    delete destination;
    destination = nullptr;
  }

  delete shader;
  shader = nullptr;

  delete player;
  player = nullptr;

  delete sun;
  sun = nullptr;
}

void SokobanApplication::drawSun() {

  // Move the sun across the sky
  sun->setPosition({sun->getPosition().x,
                    std::sin(lerp(TIMEOFDAY)) * sunDistance,
                    std::cos(lerp(TIMEOFDAY)) * sunDistance});

  // Ambient color
  auto ambientColor =
      glm::vec3(1.0f) -
      glm::vec3(std::cos(TIMEOFDAY) * 0.1f,
                (std::cos(TIMEOFDAY) + std::sin(TIMEOFDAY)) * 0.1f,
                std::sin(TIMEOFDAY * 2) * 0.25f);

  // The color temperature is colder in the evening and morning, and warmer
  // during the day
  float timeFactor = (std::sin(TIMEOFDAY) + 1) *
                     0.5f; // Some magic number I don't know what it doesw
  auto ambientColorCycle =
      glm::vec3(timeFactor, timeFactor, timeFactor) * ambientColor;
  shader->setFloat3("u_ambientColor", ambientColor * ambientColorCycle);

  // Camera position for specularity
  shader->setFloat3("u_cameraPosition", camera->getPosition());

  // Light
  light.setPosition(sun->getPosition());
  light.setEnableLighting(true);
  light.setSpecularColor(glm::vec3(std::cos(TIMEOFDAY)));
  light.setDiffuseColor(glm::vec3(std::cos(TIMEOFDAY)));
  light.update();

  // Disable lighting effects on the sun object only
  textureManager.bind("no-texture");
  shader->setInt("u_enableLighting", 0);
  sun->draw();
  shader->setInt("u_enableLighting", 1);
}
