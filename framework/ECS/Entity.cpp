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

    Entity* Entity::removeChildById(int id) {
        auto found = std::find_if(children.begin(),
                                  children.end(),
                                  [id](Entity* e) { return e->getId() == id; });

        if (found != children.end()) {
            Entity* e = *found;
            children.erase(found);
            return e;
        } else {
            return nullptr;
        }
    }

    Entity::~Entity() {
        for (auto& child : children) {
            delete child;
        }
    }
} // Framework
