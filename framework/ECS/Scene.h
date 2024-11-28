#pragma once

#include "pch.h"
#include "Entity.h"

namespace FW {
/** A scene is a game state. It is the overall supervisor of all things that
 * happens thereafter. Only one scene can be active at any time.
 * 
 * A scene can only have one direct child node. To build out the scene, users
 * must add child nodes to the scene root node.
 */
class Scene {
public:
    Scene() = default;
    virtual ~Scene();
    virtual void init();
    virtual void cleanUp() = 0;
    virtual void update(float delta);

    ref<Entity> getRoot() { return root; }
protected:
    ref<Entity> root;
};
} // namespace FW