#pragma once

#include "Framework.h"
#include "Sprite.h"

class JumpingPlatformerScene : public BaseScene {
public:
    JumpingPlatformerScene() = default;
    virtual ~JumpingPlatformerScene() = default;
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(ref<Shader> shader) { this->shader = shader; }
    ref<Shader> getShader() { return shader; }

    
private:
    ref<Sprite> playerSprite;
    ref<Shader> shader;
    ref<OrthographicCamera> camera;

private: // Physics
    Solver mySolver;
    ref<GravityForce> gravityForce;
};