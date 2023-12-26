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
#include "Shader.h"

namespace FW {

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
    class Skybox
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
        Skybox(int textureId);

        virtual ~Skybox();

        /**
         * Draw itself and all child entities.
         */
        virtual void draw(const ref<Shader>& shader);

        inline void setScale(const float s)
        {
            this->scale.x = s;
            this->scale.y = -s;
            this->scale.z = s;
            recalculateModelMatrix();
        }
        inline void setScale(const glm::vec3& s) { this->scale = s; }

        void setPosition(const glm::vec3& value) { position = value; recalculateModelMatrix(); }

        [[nodiscard]] inline const glm::vec3& getScale() const { return scale; }

    private:
        /**
         *  Recalculate the entity's model matrix.
         *
         * This needs to be called before every draw if the entity has changed
         * its transformation.
         */
        void recalculateModelMatrix();

    private:
        FW::VertexArray* vertexArray = nullptr;
        FW::IndexBuffer* indexBuffer = nullptr;
        FW::VertexBuffer* vertexBuffer = nullptr;

        /** The texture by ID to draw. */
        int textureId = -1;

        // Transformation
        glm::mat4 modelMatrix{ 1.0f };
        glm::vec3 position{ 0.0f };

        glm::vec3 scale { 50.0f };
    };
} // Framework
