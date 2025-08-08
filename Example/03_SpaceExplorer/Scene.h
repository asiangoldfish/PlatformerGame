#pragma once

#include "Framework.h"

class GameScene : public FW::BaseScene {
public:
    GameScene() = default;
    virtual ~GameScene() = default;
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(FW::ref<FW::Shader> shader) { this->shader = shader; }
    FW::ref<FW::Shader> getShader() { return shader; }

    
private:
    FW::ref<FW::Shader> shader;
    FW::ref<FW::OrthographicCamera> camera;

private: // Physics
    FW::Physics::Solver mySolver;
    FW::ref<FW::Physics::GravityForce> gravityForce;
};
