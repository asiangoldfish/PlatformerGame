#ifndef SOKOBAN_APPLICATION_H
#define SOKOBAN_APPLICATION_H

#include <string>
#include <memory>

#include "Framework.h"

// Globals
const int mapSize = 10.f;
const float tileSize = 1.0f;

// ---------
// Forward declarations
// ---------
class Floor;
class Cube;
class Map;

namespace Framework
{
    class Shader;
    class PerspectiveCamera;
    class TextureManager;
}

class SokobanApplication : public Framework::GLFWApplication {
public:
    /**
     * Constructor. Remember to call init() before using the application.
     * @param name Window class and application name
     * @param version Application version
     */
    SokobanApplication(
        const std::string& name,
        const std::string& version,
        glm::vec2 windowSize = glm::vec2(1024, 1024))
        : GLFWApplication(name, version, windowSize) {}

    ~SokobanApplication() override = default;

    /** Initialize the application. Must be called before using it */
    bool init() override;
    /** Game loop. Runs until the application quits. */
    void run() override;
    /** Shutdown procedure. Must be called before deleting the Sokoban Application object. */
    void shutdown() override;

    // ---------
    // Camera and rendering
    // ---------
    // Camera
    void rotateCamera(bool rotateRight);
    float getCameraDistance() const { return cameraDistance; }

    float getCameraSpeed() const { return cameraSpeed; }

    float getDeltaTime() const { return deltaTime; }
    Framework::PerspectiveCamera* getCamera() { return camera.get(); }
    float getWorldCenter() const { return worldCenter; }
    Framework::Shader* getShader() const { return shader; }

    // Textures
    bool getEnableTexture() { return enableTexture; }
    void setEnableTexture(const bool enable) { enableTexture = enable; }

    // -----------
    // Entities
    // -----------
    std::vector<Cube*>& getWalls() { return walls; }
    std::vector<Cube*>& getPillars() { return pillars; }
    std::vector<Cube*>& getDestinations() { return destinations; }
    std::vector<Cube*>& getBoxes() { return boxes; }

    bool moveBox(glm::vec3 direction);
    void movePlayer(glm::vec3 direction);

    // --------
    // Inputs
    // --------
    /** Called every frame */
    void keyboardInput();

    [[nodiscard]] bool getIsRightButtonPressed() const { return isRightButtonPressed; }
    void setIsRightButtonPressed(const bool isPressed) { isRightButtonPressed = isPressed; }

private:
    // ----------
    // Entities
    // ----------
    Floor* floor = nullptr;

    std::vector<Cube*> walls;
    std::vector<Cube*> boxes;
    std::vector<Cube*> pillars;
    std::vector<Cube*> destinations;

    const int maxDestinations = 6;
    const int maxPillars = 6;
    const int maxBoxes = 6;

    // -----------
    // Textures and shaders
    // -----------
    Framework::Shader* shader = nullptr;

    // -----------
    // Textures and shaders
    // -----------
    Framework::Model backpackModel;

    // -----------
    // Camera
    // -----------
    float degreesDirection = 1;
    float degrees = 10.0f;
    std::shared_ptr<Framework::PerspectiveCamera> camera;
    float cameraDistance = 20.0f;
    float cameraSpeed = 0.1f;
    float worldCenter = (float)mapSize / 2.0f;  // Position in which the camera should always look at

    Framework::DirectionalLight sun;
    Framework::PointLight pointLight;



    float deltaTime;
    float lastFrameTime;
    bool enableTexture = true;

    bool isRightButtonPressed = false;

    // ---------------
    // Map
    // ---------------
    Map* map = nullptr;

};


#endif // SOKOBAN_APPLICATION_H
