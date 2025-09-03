#pragma once

#include "BaseScene.h"
#include "Entity.h"

#include <vector>

namespace FW {
    class BaseSystem {
    public:
        BaseSystem() = default;
        virtual ~BaseSystem() = default;

        virtual void update(float delta) = 0;
    };

    class RenderSystem : public BaseSystem {
    public:
        RenderSystem();
        virtual ~RenderSystem() = default;

        virtual void update(float delta) override {};
        virtual void draw(ref<SceneNode> sceneRoot);
    };
}