#include "PhysicsScene.h"
#include "BaseScene.h"

PhysicsScene::PhysicsScene() {}

PhysicsScene::~PhysicsScene() {}

void PhysicsScene::init() {
    FW::BaseScene::init();

    selectedNode = FW::createRef<SelectedNode>();
}

void PhysicsScene::cleanUp() {}

void PhysicsScene::update(float delta) {
    FW::BaseScene::update(delta);

    // Test spawn objects
    if (assetSystem->spawnCube) {
        assetSystem->spawnCube = false;

        FW::ref<FW::PrimitiveCube> shape = FW::createRef<FW::PrimitiveCube>();
        shape->init();

        FW::ref<FW::DrawableComponent> drawableComponent =
          FW::createRef<FW::DrawableComponent>();
        drawableComponent->setShape(shape);
        drawableComponent->setShader("cube");
        drawableComponent->init();

        FW::ref<FW::TransformationComponent> xformComponent =
          FW::createRef<FW::TransformationComponent>();
        xformComponent->setShader("cube");
        xformComponent->init();

        auto sceneNode = FW::createRef<FW::SceneNode>(true);
        sceneNode->entity->addComponent(drawableComponent);
        sceneNode->entity->addComponent(xformComponent);
        sceneNode->entity->name = "Drawable Entity";

        rootNode->addChild(sceneNode);
    }
}
