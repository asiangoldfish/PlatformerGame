#include "Environment.h"

#include "ShaderManager.h"
#include "Sprite.h"

Asteroid::Asteroid(FW::ref<FW::Camera> camera) {
    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Asteroid";
    sprite->setColor(0.3f, 0.7f, 0.2f);
    sprite->setSize(50.0f);
    sprite->setPosition(40.0f, 40.0f);
    sprite->setZIndex(1);
    entity = sprite;

    INFO("Created test asteroid!");
}

void Asteroid::update(float delta) {
    FW::SceneNode::update(delta);
}