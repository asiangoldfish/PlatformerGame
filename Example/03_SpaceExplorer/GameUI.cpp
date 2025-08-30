#include "GameUI.h"
#include "assertions.h"
#include <algorithm>

void GameUI::init() {
    ASSERT(camera, "GameUI::Camera is not set!");

    uiRoot = FW::createRef<FW::Sprite>();
    healthBar = FW::createRef<FW::UI::ProgressBar>(camera);
    uiRoot->addChild(healthBar);
    INFO("GameUI successfully initialised");
}

void GameUI::draw(float delta) {}

void GameUI::invokeAllHandlers() {
    for (auto& [id, handler] : handlers) {
        handler();
    }
}

void GameUI::addHandler(const std::string& id, std::function<void()> func) {
    auto it = std::find_if(handlers.begin(),
                           handlers.end(),
                           [id](auto handler) { return handler.first == id; });

    if (it == handlers.end()) {
        handlers.emplace_back(id, std::move(func));
    }
}

void GameUI::removeHandler(const std::string& id) {
    std::erase_if(handlers, [id](auto pair) { return pair.first == id; });
}
