#pragma once

#include "Ship.h"

#include <GLFW/glfw3.h>

class Debugging {
public:
    Debugging() = default;
    virtual ~Debugging();

    void init(GLFWwindow* window);

    void draw(float delta);

private:
    void drawShipStats();

public:
    FW::ref<PlayerShip> playerShip;
    FW::ref<EnemyShip> enemyShip;

private:
    GLFWwindow* window = nullptr;
    float fontSize = 10.0f;
};