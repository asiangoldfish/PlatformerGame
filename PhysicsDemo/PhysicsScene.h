#pragma once

#include "pch.h"
#include "Framework.h"

class PhysicsScene : public FW::Scene {
public:
    PhysicsScene();
    virtual ~PhysicsScene();
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(FW::ref<FW::Shader> shader) { this->shader = shader; }
    FW::ref<FW::Shader> getShader() { return shader; }
    
private:
    FW::Entity* drawableEntity;
    FW::ref<FW::Shader> shader;
};