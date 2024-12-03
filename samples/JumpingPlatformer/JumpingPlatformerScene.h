#pragma once

#include "Framework.h"
#include "Sprite.h"

class JumpingPlatformerScene : public FW::BaseScene {
public:
    JumpingPlatformerScene() = default;
    virtual ~JumpingPlatformerScene() = default;
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(FW::ref<FW::Shader> shader) { this->shader = shader; }
    FW::ref<FW::Shader> getShader() { return shader; }
    
private:
    FW::ref<Sprite> playerSprite;
    FW::ref<FW::Shader> shader;
    FW::ref<FW::OrthographicCamera> camera;
};