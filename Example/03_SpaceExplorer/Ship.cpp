#include "Ship.h"

#include "SpaceExplorerUtils.h"

#include "Math/Math.h"

#include <glm/gtx/norm.hpp>

#include <algorithm>
#include <random>

FW::ref<Bullet> createBullet(FW::ref<FW::Camera> camera) {
    FW::ref<Bullet> bullet = FW::createRef<Bullet>();

    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Bullet";
    sprite->setColor(1.0f, 0.3f, 0.75f);
    sprite->setSize(30, 10);
    sprite->setPosition(0.0f, 0.0f);

    bullet->entity = sprite;

    return bullet;
}

Ship::Ship(FW::ref<FW::Camera> camera) {
    this->camera = camera;
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

void Ship::setPosition(glm::vec2 pos) {
    setPosition(pos.x, pos.y);
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

void Ship::update(float delta) {
    FW::SceneNode::update(delta);
    INFO("Position: ({}, {})\r", getPosition().x, getPosition().y);

    if (fireCurrentCooldown >= 0.0f) {
        fireCurrentCooldown -= delta;
    }

    glm::vec2 pos = getPosition();
    glm::vec2 posDelta = glm::vec2{0.0f};

    // Moving the ship
    if (FW::Input::isKeyPressed(FW_KEY_D)) {
        posDelta.x += speed;
    }
    if (FW::Input::isKeyPressed(FW_KEY_A)) {
        posDelta.x -= speed;
    }
    if (FW::Input::isKeyPressed(FW_KEY_S)) {
        posDelta.y -= speed;
    }
    if (FW::Input::isKeyPressed(FW_KEY_W)) {
        posDelta.y += speed;
    }
    setPosition(pos + posDelta);
}

void Ship::fireBullets(FW::ref<FW::SceneNode> root) {
    if (fireCurrentCooldown >= 0.0f) {
        return;
    }

    glm::vec2 playerPos = getPosition();
    float angle = -getRotationWithMouse().z;
    float speed = 1200.0f;
    auto bullet = createBullet(camera);

    // Compute bullet spread
    float spreadInaccuracy = (1.0f - accuracy) / 2.0f;
    float minSpread = 1.0f - spreadInaccuracy;
    float maxSpread = 1.0f + spreadInaccuracy;

    // 3. Generate and print a random float
    // TODO Fix inaccuracy
    // Currently, the inaccuracy is greater the greater the angle is. This is
    // because the closer to 0 it is, the less the variance. The greater the
    // angle, proportionally the greater the angle variance is. Therefore, the
    // inaccuracy should be in radians. We can still have the accuracy in
    // percent, but we have to convert it to radian in the computation, i.e. 2PI
    // * accuracy.
    angle *= FW::rng(minSpread, maxSpread);

    bullet->setPosition(playerPos);
    bullet->velocity = glm::vec2{ cos(angle) * speed, sin(angle) * speed };
    bullet->setRotation(glm::vec3{ 0.0f, 0.0f, angle });
    root->addChild(bullet);

    fireCurrentCooldown = fireMaxCooldown;
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
        return bulletScene && bulletScene->time >= bulletScene->maxTime;
    });
}
