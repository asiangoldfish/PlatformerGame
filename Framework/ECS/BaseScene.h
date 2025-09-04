#pragma once

#include "Entity.h"
#include "Viewport.h"

#include <vector>

namespace FW {
    class SceneNode {
    public:
        virtual void update(float delta);
        virtual void addChild(ref<SceneNode> node);
        virtual void removeChild(ref<SceneNode> node);

    public:
        ref<Entity> entity;
        std::vector<ref<SceneNode>> childNodes;
    };

    /** A scene is a game state. It is the overall supervisor of all things that
     * happens thereafter. Only one scene can be active at any time.
     *
     * A scene can only have one direct child node. To build the scene, users
     * must add child nodes to the scene root node. This node is of type \ref
     * SceneNode "SceneNode".
     */
    class BaseScene {
    public:
        BaseScene() = default;
        virtual ~BaseScene();
        virtual void init();
        virtual void cleanUp() = 0;
        virtual void update(float delta);

        ref<SceneNode> getRoot() { return rootNode; }
        ref<Viewport> getViewport() { return viewport; }

    protected:
        ref<SceneNode> rootNode;

    private:
        ref<Viewport> viewport;
    };
} // namespace FW