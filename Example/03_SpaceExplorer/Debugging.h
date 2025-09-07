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
    void drawCurrencies();

public:
    std::weak_ptr<PlayerShip> playerShip;

private:
    GLFWwindow* window = nullptr;
    float fontSize = 10.0f;
};