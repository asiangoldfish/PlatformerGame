#pragma once

#include "pch.h"
#include "Framework.h"
#include "Selections.h"
#include "AssetSystem.h"

class PhysicsScene : public FW::BaseScene {
public:
    PhysicsScene();
    virtual ~PhysicsScene();
    virtual void init();
    virtual void cleanUp();
    virtual void update(float delta) override;

    void setShader(std::string shader) { this->shader = shader; }
    std::string getShader() { return shader; }
    
    void setAssetSystem(FW::ref<AssetSystem> assetSystem) {
        this->assetSystem = assetSystem;
    }

public:
    FW::ref<SelectedNode> selectedNode;

private:
    /** Selectable entity that is shown in the properties panel */
    std::string shader;
    FW::ref<AssetSystem> assetSystem;
};