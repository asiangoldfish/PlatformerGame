#pragma once

#include "Framework.h"
#include "Sprite.h"

class GameScene : public FW::BaseScene {
public:
    GameScene() = default;
    virtual ~GameScene() = default;
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

private:
    FW::ref<FW::OrthographicCamera> camera;
    FW::ref<Sprite> playerSprite;

private: // Physics
    FW::Physics::Solver mySolver;
    FW::ref<FW::Physics::GravityForce> gravityForce;
};
