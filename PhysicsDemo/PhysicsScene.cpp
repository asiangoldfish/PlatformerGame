#include "PhysicsScene.h"
#include "BaseScene.h"

PhysicsScene::PhysicsScene() {}

PhysicsScene::~PhysicsScene() {}

void PhysicsScene::init() {
    FW::BaseScene::init();

    FW::ref<FW::PrimitiveCube> shape = FW::createRef<FW::PrimitiveCube>();
    shape->init();

    FW::ref<FW::DrawableComponent> drawableComponent =
      FW::createRef<FW::DrawableComponent>();
    drawableComponent->setShape(shape);
    drawableComponent->setShader(shader);
    drawableComponent->init();

    FW::ref<FW::TransformationComponent> xformComponent =
      FW::createRef<FW::TransformationComponent>();
    xformComponent->setShader(shader);
    xformComponent->init();

    drawableEntity = new FW::Entity();
    drawableEntity->addComponent(drawableComponent);
    drawableEntity->addComponent(xformComponent);
    drawableEntity->name = "Drawable Entity";
    root->addChild(drawableEntity);
}

void PhysicsScene::cleanUp() {}

void PhysicsScene::update(float delta) {
    FW::BaseScene::update(delta);
}
