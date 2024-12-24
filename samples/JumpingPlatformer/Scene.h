#pragma once

#include "Framework.h"

class JumpingPlatformerScene : public BaseScene {
public:
    JumpingPlatformerScene() = default;
    virtual ~JumpingPlatformerScene() = default;

    /**
     * Initialise parameters. It is primarily used to add and instantiate nodes
     * in the Scene.
     */
    void init() override;

    /**
     * If you have added something that requires clean up, like heap allocating
     * objects, you can do that here. Most Engine objects like Sprites and
     * other nodes are automatically memory managed.
     */
    void cleanUp() override;

    /**
     * This is your game loop. It is run once per frame. It is the Application
     * class that calls this method.
     *
     * @param delta the time between frames. You can use this to create frame
     *              independent animations.
     */
    void update(float delta) override;

    void setShader(ref<Shader> shader) { this->shader = shader; }
    ref<Shader> getShader() { return shader; }

private:
    /// The Physics Engine is not a mandatory component of the Scene. You can
    /// use it if you want to create physics based animations.
    scope<PhysicsEngine> physicsEngine;

    /// This is the sprite controlled by the player.
    ref<Sprite> playerSprite;

    /// The shader lets us draw things on the screen. You can write or edit the
    /// shader code in the resources/shaders directory.
    ref<Shader> shader;

    /// We use the orthographic camera for 2d games.
    ref<OrthographicCamera> camera;
};