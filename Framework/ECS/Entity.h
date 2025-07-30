#pragma once

#include "pch.h"

// External
#include "glad/glad.h"
#include "glm/glm.hpp"

// Framework
#include "Material.h"
#include "Physics.h"
#include "Component.h"

namespace FW {

    class Shader;
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;

    /**
     * Object that exists in the scene
     *
     * The Entity is the lowest level component of the scene hierarchy. It is
     * placeable in the world either as a drawble object or as an invisible
     * entity. Such entity can be used for
     */
    class Entity {
    public: // Constructors and destructors
        /**
         * Default constructor.
         *
         * Construct an empty entity. If the entity is drawable, it must
         * first be iniitialized.
         */
        Entity();

        virtual ~Entity();

        std::vector<ref<Entity>>& getChildren() { return children; }

        /**
         * Add a new parent.
         *
         * The entity is a node in a tree structure. If it doesn't have a
         * parent, then it's safe to assume that it's the top node.
         *
         * @param entity The new parent
         */
        void setParent(Entity* entity) { parent = entity; }

        /**
         *  Add a new child
         *
         * The entity is a node in a tree structure. By adding a child, we are
         * expanding the tree structure.
         */
        void addChild(ref<Entity> entity) { children.push_back(entity); entity->setParent(this); }

        /**
         * Remove a child at the i'th index.
         *
         * Please be cautious that removing a child will not delete it. It must
         * manually be deleted by the user.
         */
        ref<Entity> removeChildAt(int i) {
            if (i < children.size()) {
                ref<Entity> removedChild = children[i];
                children.erase(children.begin() + i);
                return removedChild;
            }
        }

        /**
         * Remove a child by its id.
         *
         * Please be cautious that removing a child will not delete it. It must
         * manually be deleted by the user.
         */
        ref<Entity> removeChildByUUID(std::string UUID);

        /**
         * Return the entity's unique identifier.
         *
         * No other entity should have this identifier.
         */
        [[nodiscard]] std::string getUUID() const { return UUID; }

        void setUUID(std::string UUID) { this->UUID = UUID; }

        /**
         * Update itself and all child entities.
         *
         * This funuction should ideally be called at least once per frame.
         * This allows updating the entities internal logic. The function may
         * be overwritten by sub-classes, but it is important that the base
         * class update() is always called at the end. Example:
         *
         * void SubClass::update() {
         *    // Perform some instructions
         *    Entity::update();
         * }
         */
        virtual void update(float delta);

        void addComponent(ref<Component> component) {
            components.push_back(component);
        }

        /** Get the firstly found component by type `T`. */
        ref<Component> getComponent(std::string componentName);

    public:
        /// The node's unique name. It is display name and identifier.
        std::string name;

    private:
        Entity* parent = nullptr;
        std::vector<ref<Entity>> children;

        /** 
         * The UUID is auto-generated on instantiation. It can still be
         * overridden, but users must handle collisions manually.
         */
        std::string UUID;

        std::vector<ref<Component>> components;
    };
} // FW
