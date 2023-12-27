#pragma once

// C libraries
#include <cstdint>

// C++ libraries
#include <algorithm>
#include <vector>
#include <string>

// External
#include "glad/glad.h"
#include "glm/glm.hpp"

// Framework
#include "Material.h"
#include "Physics.h"

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
    class Entity
    {
    public: // Constructors and destructors
        /**
         * Default constructor.
         *
         * Construct an empty entity. If the entity is drawable, it must
         * first be iniitialized.
         *
         * @see initDrawable()
         */
        Entity() = default;

        virtual ~Entity();

        std::vector<Entity*>& getChildren() { return children; }

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
        void addChild(Entity* entity) { children.push_back(entity); }

        /**
         * Remove a child at the i'th index.
         *
         * Please be cautious that removing a child will not delete it. It must
         * manually be deleted by the user.
         */
        Entity* removeChildAt(int i)
        {
            if (i < children.size()) {
                return *children.erase(children.begin() + i);
            }
        }

        /**
         * Remove a child by its id.
         *
         * Please be cautious that removing a child will not delete it. It must
         * manually be deleted by the user.
         */
        Entity* removeChildById(int id)
        {
            std::find_if(children.begin(), children.end(), [id](Entity* e) {
                return e->getId() == id;
            });

            return nullptr;
        }

        /**
         * Return the entity's unique identifier.
         *
         * No other entity should have this identifier.
         */
        [[nodiscard]] int getId() const { return id; }

        /**
         * Draw itself and all child entities.
         */
        virtual void draw(const ref<Shader>& shader);

        /**
         * Update itself and all child entities.
         *
         * This funuction should ideally be called at least once per frame.
         * This allows updating the entities internal logic. The function may be
         * overwritten by sub-classes, but it is important that the base class
         * update() is always called at the end. Example:
         *
         * void SubClass::update() {
         *    // Perform some instructions
         *    Entity::update();
         * }
         */
        virtual void update();

        void setRotation(glm::vec3 rotation);
        void setRotation(float yaw, float pitch, float roll);

        inline void setScale(const float scale)
        {
            this->scale.x = scale;
            this->scale.y = scale;
            this->scale.z = scale;
            recalculateModelMatrix();
        }
        inline void setScale(const glm::vec3& scale) { this->scale = scale; }

        inline const glm::vec3& getScale() const { return scale; }

        inline void setPosition(glm::vec3 position)
        {
            this->position = position;
            recalculateModelMatrix();
        }
        const glm::vec3& getPosition() const { return position; }

        const glm::vec4& getColor() const { return color; }

        /**
         * Get the entity's material
         */
        Material& getMaterial() { return material; }

        /**
         * Set the entity's material.
         *
         * The material is set to MaterialPreset::CUSTOM by default.
         * Change the material's properties for a desired result.
         * @param mat Material to set
         */
        void setMaterial(const Material& mat) { material = mat; }

        /**
         * Set the entity's colour.
         *
         * If the entity has a texture, then these will be mixed before
         * rendering.
         */
        void setColor(const glm::vec4& color);

        /**
         * Set the entity's colour. The alpha channel is set to 1.0.
         *
         * If the entity has a texture, then these will be mixed before
         * rendering.
         */
        void setColor(const glm::vec3& color);

        /**
         * Set the entity's colour. The alpha channel is set to 1.0.
         *
         * The colour will be on a gray scale.
         * If the entity has a texture, then these will be mixed before
         * rendering.
         */
        void setColor(const float color);

        const std::vector<glm::vec3> getVertices();

        // Physics
        BoundingBox& getBoundingBox() { return boundingBox; }

        void setTextureName(const std::string& name) { textureName = name; }

    public:
        /**
         *  Recalculate the entity's model matrix.
         *
         * This needs to be called before every draw if the entity has changed
         * its transformation.
         */
        void recalculateModelMatrix();

    protected:
        /**
         * Initialize required parameters and data to draw the entity on screen.
         *
         * @param shader The shader to bind to before drawing the entity
         * @param vertices Vertices with attributes. These are used when drawing
         * @param indices All indices. Used for the index buffer
         * @param drawType (default: GL_DYNAMIC_DRAW) Either static or dynamic
         */
        void initDrawable(std::vector<float> vertices,
                          std::vector<uint32_t> indices);

    private:
        Entity* parent = nullptr;
        std::vector<Entity*> children;

        /** Unique identifier. No other entity should have this id */
        int id = -1;

        /** If this is set, then bind a texture by name before drawing. */
        std::string textureName;

        /** Prevents drawing itself if this isn't configured yet */
        bool isDrawable = false;

    protected:
        VertexArray* vertexArray = nullptr;
        VertexBuffer* vertexBuffer = nullptr;
        IndexBuffer* indexBuffer = nullptr;

        // Transformation
        glm::mat4 modelMatrix;
        glm::vec3 position;
        float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;

        // This color is multiplied by texture color
        glm::vec4 color{ 1.0f, 1.0f, 1.0f, 1.0f };

        std::vector<float> vertices;
        std::vector<uint32_t> indices;

        glm::vec3 scale = glm::vec3{ 1.0f };

        GLenum drawType;

        // Disable per-entity-basis
        bool enableDepthTesting = true;

        // Physics
        BoundingBox boundingBox;

        // ----------
        // Materials and textures
        // ----------
        Material material;
    };
} // FW
