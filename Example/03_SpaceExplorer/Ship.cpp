#include "Ship.h"

Ship::Ship(FW::ref<FW::Camera> camera) {
    sprite = FW::createRef<FW::Sprite>(camera);
    sprite->setColor(0.2f, 0.8f, 0.1f);
    sprite->setSize(100, 50);
    sprite->setPosition(600.f, 400.f);
    INFO("Ship successfully initialised");
}

void Ship::update(float delta) {
    // camera->update(sprite->getShader());
}