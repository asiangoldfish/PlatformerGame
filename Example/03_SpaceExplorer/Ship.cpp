#include "Ship.h"

Ship::Ship() {
    sprite = FW::createRef<FW::Sprite>();
    sprite->setSize(100, 50);
    sprite->setPosition(600.f, 400.f);
}

void Ship::update(float delta) {
    camera->update(sprite->getShader());
}