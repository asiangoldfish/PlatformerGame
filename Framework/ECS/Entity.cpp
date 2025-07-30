// ---------
// External libraries
// ---------
#include "glm/gtc/matrix_transform.hpp"

//----------
// Framework
//----------
#include "Log.h"

// OpenGL
#include "Buffer.h"

// Rendering
#include "RenderCommands.h"
#include "Shader.h"
#include "TextureManager.h"

// Entity
#include "Entity.h"

// Util
#include "FW_random.h"

namespace FW {
    // Recursively update itself and child nodes
    void Entity::update(float delta) {
        for (auto& component : components) {
            component->update(delta);
        }

        for (const auto& child : children) {
            child->update(delta);
        }
    }

    ref<Component> Entity::getComponent(std::string componentName) {
        auto foundIterator = std::find_if(components.begin(), components.end(), [&](ref<Component> c) {
            return c->name == componentName; });

        return *foundIterator;
    }

    ref<Entity> Entity::removeChildByUUID(std::string UUID) {
        auto found = std::find_if(children.begin(),
                                  children.end(),
                                  [UUID](ref<Entity> e) { return e->getUUID() == UUID; });

        if (found != children.end()) {
            ref<Entity> e = *found;
            children.erase(found);
            return e;
        } else {
            return nullptr;
        }
    }

    Entity::Entity() {
        UUID = generateUUID();
    }

    Entity::~Entity() {}
} // Framework
