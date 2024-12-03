#pragma once

#include "Framework.h"
#include "Sprite.h"

class JumpingPlatformerScene : public FW::BaseScene {
public:
    JumpingPlatformerScene();
    virtual ~JumpingPlatformerScene();
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(FW::ref<FW::Shader> shader) { this->shader = shader; }
    FW::ref<FW::Shader> getShader() { return shader; }
    
private:
    FW::ref<Sprite> drawableEntity;
    FW::ref<FW::Shader> shader;
};