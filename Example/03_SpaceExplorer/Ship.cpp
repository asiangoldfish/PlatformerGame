#include "Ship.h"

#include "SpaceExplorerUtils.h"

#include "Math/Math.h"

#include <glm/gtx/norm.hpp>

#include <algorithm>
#include <random>
#include <cmath>

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

Ship::Ship(FW::ref<FW::Camera> camera, FW::ref<ProjectileRoot> projectileRoot)
  : camera(camera)
  , projectileRoot(projectileRoot) {

    FW::ref<FW::Sprite> sprite = FW::createRef<FW::Sprite>(camera);
    sprite->name = "Spaceship";
    // sprite->setColor(0.2f, 0.8f, 0.1f);
    sprite->setSize(150.0f);
    sprite->setTexture("spaceship",
                       TEXTURES_DIR + std::string("spaceship_sprite.png"));
    // sprite->setPosition(600.f, 400.f);
    entity = sprite;

    // Create a target selector child node
    targetSelectorScene = FW::createRef<FW::SceneNode>();

    FW::ref<FW::Sprite> targetSelectorSprite = targetSelectorSprite =
      FW::createRef<FW::Sprite>(camera);
    targetSelectorSprite->name = "Target Selector";
    targetSelectorSprite->setColor(1.0f, 0.2f, 0.2f);
    targetSelectorSprite->setSize(200.f);
    targetSelectorSprite->setPosition(sprite->getPosition());
    targetSelectorSprite->setZIndex(3);

    targetSelectorSprite->getComponent<FW::DrawableComponent>()->setShader(
      "Target selector");
    targetSelectorSprite->getComponent<FW::TransformationComponent>()
      ->setShader("Target selector");

    targetSelectorScene->entity = targetSelectorSprite;

    INFO("Ship successfully initialised");
}

void Ship::setPosition(float x, float y) {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return;
    }

    xformComponent->setPosition(x, y);
}

void Ship::setPosition(float x, float y, float z) {
    auto xformComponent = entity->getComponent<FW::TransformationComponent>();

    if (!xformComponent) {
        return;
    }

    xformComponent->setPosition(x, y, z);
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

    if (fireCurrentCooldown >= 0.0f) {
        fireCurrentCooldown -= delta;
    }

    if (isTargeted) {
        targetSelectorScene->entity->getComponent<FW::TransformationComponent>()
          ->setPosition(getPosition());
    }

    // Ships will turn toward their target
    if (targetShip) {
        float angle = angleToEnemy(this, targetShip.get());
        setRotation({ 0.0f, 0.0f, angle });
    }
}

void Ship::fireBullets(FW::ref<FW::SceneNode> root) {
    if (fireCurrentCooldown >= 0.0f) {
        return;
    }

    glm::vec2 playerPos = getPosition();

    // If we have a target, then rotate toward it. Otherwise, point toward the
    // mouse's position on screen.
    float angle = targetShip ? angleToEnemy(this, targetShip.get())
                             : -getRotationWithMouse().z;
    float speed = 1200.0f;
    auto bullet = createBullet(camera);

    // Compute bullet spread
    float randomSpread = FW::remap(FW::rng(),
                                   0.0f,
                                   1.0f,
                                   -randomSpreadRadius / 2.0f,
                                   randomSpreadRadius / 2.0f);

    bullet->setPosition(playerPos);
    bullet->velocity = glm::vec2{ cos(angle + randomSpread) * speed,
                                  sin(angle + randomSpread) * speed };
    bullet->setRotation(glm::vec3{ 0.0f, 0.0f, angle });
    root->addChild(bullet);

    fireCurrentCooldown = fireMaxCooldown;
}

void Ship::setZIndex(uint32_t z) {
    std::dynamic_pointer_cast<FW::Sprite>(entity)->setZIndex(z);
}

void Ship::setIsTargeted(const bool b) {
    isTargeted = b;
    if (b) {
        addChild(targetSelectorScene);
    } else {
        removeChild(targetSelectorScene);
    }
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

void PlayerShip::update(float delta) {
    Ship::update(delta);

    // Only rotate if we haven't set a target to shoot or chase yet
    if (!targetShip) {
        setRotation(-getRotationWithMouse());
    }

    glm::vec2 pos = getPosition();
    glm::vec2 posDelta = glm::vec2{ 0.0f };

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

    if (FW::Input::isMouseButtonPressed(FW_MOUSE_BUTTON_LEFT)) {
        fireBullets(projectileRoot);
    }
}

void EnemyShip::update(float delta) {
    Ship::update(delta);
}
