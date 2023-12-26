#pragma once

#include <vector>
#include <memory>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace Framework {
    /**
     * Vertex Array Object in OpenGL.
     *
     * The Vertex Array class represents the Vertex Array Object in OpenGL.
     * It is a container for Vertex Buffer Objects and Element Buffer Objects.
     *
     * Note that this class supports several Vertex Buffer Objects, but only one
     * Element Buffer Object.
     */
    class VertexArray {
    public:
        /** Construct a Vertex Array Object. */
        VertexArray();
        virtual ~VertexArray();

        /** Bind the Vertex Array Object */
        void bind() const;

        /** Unbind the Vertex Array Object */
        void unbind() const;

        /**
         * Add a new Vertex Buffer Object
         * @param vertexBuffer Shared pointer to the Vertex Buffer Object
         */
        void addVertexBuffer(VertexBuffer* vertexBuffer);

        /**
         * Set the Element Buffer Object-
         * @param indexBuffer Element Buffer Object to set for this Vertex
         * Array Object.
         */
        void setIndexBuffer(IndexBuffer* indexBuffer);

        /**
         * Get a reference to the Element Buffer Object.
         * @return Shared pointer to the Element Buffer Object
         */
        const IndexBuffer* getIndexBuffer() const {
            return indexBuffer;
        }

        /**
         * Shortcut to create a new shared pointer to Vertex Array Object.
         * @return
         */
        static std::shared_ptr<VertexArray> create();

    private:
        /** This VAO's ID. Used to reference it in GLAD functions */
        uint32_t vertexArrayID = 0;

        /** Container with VBOs associated with this VAO */
        std::vector<VertexBuffer*> vertexBuffers;

        /** This VAO's associated EBO. */
        IndexBuffer* indexBuffer = nullptr;
    };
}
