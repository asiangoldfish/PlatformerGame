#pragma once

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

namespace FW {
    class Shader;
    class PerspectiveCamera;
    class TextureManager;
}

class SokobanApplication : public FW::GLFWApplication
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

    virtual ~SokobanApplication();

    /** Initialize the application. Must be called before using it */
    bool init() override;
    /** Game loop. Runs until the application quits. */
    void run() override;

    // ---------
    // Camera and rendering
    // ---------
    // Camera
    [[nodiscard]] FW::ref<FW::CameraController>& getCameraController()
    {
        return cameraController;
    }

    [[nodiscard]] FW::ref<FW::Shader> getShader() { return shader; }

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
    FW::ref<FW::Shader> shader;
    FW::ref<FW::Shader> skyboxShader;
    FW::ref<FW::Skybox> skybox;

    FW::ref<FW::Model> backpackModel;
    FW::ref<Cube> testCube;
    FW::ref<Cube> player;

    FW::ref<FW::CameraController> cameraController;

    FW::DirectionalLight sun;
    FW::PointLight pointLight;

    FW::Timer timer;

    bool enableTexture = true;
    bool isRightButtonPressed = false;

    FW::ref<Map> map;
};

FW::GLFWApplication* FW::createApplication()
{
    return new SokobanApplication("Game", "1.0.0", { 1280, 720 });
}
