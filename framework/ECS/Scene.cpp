#include "Scene.h"

FW::Scene::~Scene() {
    delete root;
}

void FW::Scene::init() {
    root = new Entity();
    root->name = "root";
}

void FW::Scene::update(float delta) {
    root->update(delta);
}