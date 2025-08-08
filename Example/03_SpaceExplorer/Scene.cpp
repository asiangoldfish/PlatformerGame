#include "Scene.h"

void GameScene::init() {
    FW::BaseScene::init();

    camera = FW::createRef<FW::OrthographicCamera>();
    camera->setCameraSize(1280.0f, 720.0f);

}

void GameScene::update(float delta) {
    FW::BaseScene::update(delta);
}

void GameScene::cleanUp() {}
