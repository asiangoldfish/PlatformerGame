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

namespace Framework {
    class Shader;
    class PerspectiveCamera;
    class TextureManager;
}

class SokobanApplication : public Framework::GLFWApplication
{
public:
    /**
     * Constructor. Remember to call init() before using the application.
     * @param name Window class and application name
     * @param version Application version
     */
    SokobanApplication(const std::string& name,
                       const std::string& version,
                       glm::vec2 windowSize = glm::vec2(1024, 1024))
      : GLFWApplication(name, version, windowSize)
    {
    }

    ~SokobanApplication() override = default;

    /** Initialize the application. Must be called before using it */
    bool init() override;
    /** Game loop. Runs until the application quits. */
    void run() override;
    /** Shutdown procedure. Must be called before deleting the Sokoban
     * Application object. */
    void shutdown() override;

    // ---------
    // Camera and rendering
    // ---------
    // Camera
    [[nodiscard]] Framework::ref<Framework::CameraController>& getCameraController()
    {
        return cameraController;
    }

    [[nodiscard]] Framework::Shader* getShader() const { return shader; }

    // Textures
    bool getEnableTexture() const { return enableTexture; }
    void setEnableTexture(const bool enable) { enableTexture = enable; }

    // --------
    // Inputs
    // --------
    /** Called every frame */
    void keyboardInput();

    [[nodiscard]] bool getIsRightButtonPressed() const
    {
        return isRightButtonPressed;
    }
    void setIsRightButtonPressed(const bool isPressed)
    {
        isRightButtonPressed = isPressed;
    }

private:
    // ----------
    // Entities
    // ----------
    Floor* floor = nullptr;

    // -----------
    // Textures and shaders
    // -----------
    Framework::Shader* shader = nullptr;

    Framework::Shader* skyboxShader = nullptr;
    Framework::Skybox* skybox = nullptr;

    // -----------
    // Models
    // -----------
    Framework::Model backpackModel;
    Cube* testCube;

    // -----------
    // Camera
    // -----------
    Framework::ref<Framework::CameraController> cameraController;

    Framework::DirectionalLight sun;
    Framework::PointLight pointLight;

    Framework::Timer timer;

    bool enableTexture = true;

    bool isRightButtonPressed = false;

    // ---------------
    // Map
    // ---------------
    Map* map = nullptr;
};

#endif // SOKOBAN_APPLICATION_H
