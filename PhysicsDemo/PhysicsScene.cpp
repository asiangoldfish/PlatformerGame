#include "PhysicsScene.h"
#include "Scene.h"

PhysicsScene::PhysicsScene() {}

PhysicsScene::~PhysicsScene() {}

void PhysicsScene::init() {
    FW::Scene::init();

    drawableEntity = new FW::Entity();
    drawableEntity->setColor(1.0);
    drawableEntity->name = "Drawable Entity";
    root->addChild(drawableEntity);
}

void PhysicsScene::cleanUp() {}

void PhysicsScene::update(float delta) {
    FW::Scene::update(delta);
}
