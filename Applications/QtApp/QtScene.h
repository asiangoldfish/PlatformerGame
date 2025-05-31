#pragma once

#include "pch.h"
#include "Framework.h"

class QtScene : public BaseScene {
public:
    QtScene();
    virtual ~QtScene();
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(ref<Shader> shader) { this->shader = shader; }
    ref<Shader> getShader() { return shader; }
    
private:
    ref<Entity> drawableEntity;
    ref<Shader> shader;
};