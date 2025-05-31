#include "QtScene.h"
#include "BaseScene.h"

QtScene::QtScene() {}

QtScene::~QtScene() {}

void QtScene::init() {
    BaseScene::init();

    ref<PrimitiveCube> shape = createRef<PrimitiveCube>();
    shape->init();

    ref<DrawableComponent> drawableComponent =
      createRef<DrawableComponent>();
    drawableComponent->setShape(shape);
    drawableComponent->setShader(shader);
    drawableComponent->init();

    ref<TransformationComponent> xformComponent =
      createRef<TransformationComponent>();
    xformComponent->setShader(shader);
    xformComponent->init();

    drawableEntity = createRef<Entity>();
    drawableEntity->addComponent(drawableComponent);
    drawableEntity->addComponent(xformComponent);
    drawableEntity->name = "Drawable Entity";
    root->addChild(drawableEntity);
}

void QtScene::cleanUp() {}

void QtScene::update(float delta) {
    BaseScene::update(delta);
}
