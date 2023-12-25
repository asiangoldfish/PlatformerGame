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

    [[nodiscard]] Framework::ref<Framework::Shader> getShader() { return shader; }

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
    // -----------
    // Textures and shaders
    // -----------
    Framework::ref<Framework::Shader> shader = nullptr;
    Framework::ref<Framework::Shader> skyboxShader = nullptr;
    Framework::ref<Framework::Skybox> skybox = nullptr;

    // -----------
    // Models
    // -----------
    Framework::ref<Framework::Model> backpackModel;
    Framework::ref<Cube> testCube;

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
    Framework::ref<Map> map;
};

#endif // SOKOBAN_APPLICATION_H
