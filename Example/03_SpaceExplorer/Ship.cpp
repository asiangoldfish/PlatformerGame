#include "Ship.h"

#include <glm/gtx/norm.hpp>
#include <algorithm>

Ship::Ship(FW::ref<FW::Camera> camera) {
    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Player Spaceship";
    sprite->setColor(0.2f, 0.8f, 0.1f);
    sprite->setSize(100, 50);
    sprite->setPosition(600.f, 400.f);
    entity = sprite;

    INFO("Ship successfully initialised");
}

void Ship::setPosition(float x, float y) {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return;
    }

    xformComponent->setPosition(x, y);
}

glm::vec2 Ship::getPosition() {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return glm::vec2{ 0.0f };
    }

    glm::vec3 out = xformComponent->getPosition();
    return glm::vec2{ out.x, out.y };
}

void Ship::setRotation(glm::vec3 rot) {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return;
    }

    xformComponent->setRotation(rot.x, rot.y, rot.z);
}

void Bullet::update(float delta) {
    FW::SceneNode::update(delta);

    if (glm::length2(velocity) > 0.0f) {
        glm::vec2 currentPos = getPosition();
        glm::vec2 newPos = currentPos + (velocity * delta);
        setPosition(newPos);
        time += delta;
    }
}

void Bullet::setPosition(float x, float y) {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return;
    }

    xformComponent->setPosition(x, y);
}
void Bullet::setPosition(glm::vec2 pos) {
    setPosition(pos.x, pos.y);
}

glm::vec2 Bullet::getPosition() {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return glm::vec2{ 0.0f };
    }

    glm::vec3 out = xformComponent->getPosition();
    return glm::vec2{ out.x, out.y };
}

void Bullet::setRotation(glm::vec3 rot) {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return;
    }

    xformComponent->setRotation(rot.x, rot.y, rot.z);
}

void ProjectileRoot::update(float delta) {
    FW::SceneNode::update(delta);

    std::erase_if(childNodes, [](const FW::ref<SceneNode>& child) {
        auto bulletScene = std::dynamic_pointer_cast<Bullet>(child);
        return bulletScene && bulletScene->time >= bulletScene->maxTime;;
    });
}
