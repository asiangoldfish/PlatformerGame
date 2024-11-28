#include "Scene.h"

FW::Scene::~Scene() {
}

void FW::Scene::init() {
    root = createRef<Entity>();
    root->name = "root";
}

void FW::Scene::update(float delta) {
    root->update(delta);
}