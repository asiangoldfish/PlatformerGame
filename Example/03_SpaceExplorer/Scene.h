#pragma once

#include "Framework.h"
#include "Ship.h"

class GameScene : public FW::BaseScene {
public:
    GameScene() = default;
    virtual ~GameScene() = default;
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void keyCallback(int key, int scancode, int action, int mods);
    void cursorPosCallback(double xpos, double ypos);
    void mouseButtonCallback(int button, int action, int mods);

private:
    FW::ref<FW::OrthographicCamera> camera;
    FW::ref<Ship> playerShip;
};
