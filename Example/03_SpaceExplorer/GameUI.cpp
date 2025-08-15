#include "GameUI.h"

GameUI::GameUI() {
    uiRoot = FW::createRef<FW::Entity>();
}

void GameUI::draw(float delta) {
    testBox.update(delta);
    camera->update(testBox.getShader());
}