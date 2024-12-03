#pragma once

#include "pch.h"

#include <glad/glad.h>

namespace FW {
    class VertexArray;
    class VertexBuffer;
    class IndexBuffer;

    class PrimitiveQuad;
    class PrimitiveCube;

    /**
     * The base shape for primitive geometries.
     *
     * @details The primitive shape forms the basis for all primitive
     * geometries. This may include quads and disks. It handles the low level
     * implementation and manages OpenGL objects lifecycles. Shapes that inherit
     * this class implement their own geometries.
     */
    class Shape {
    public:
        Shape() =  default;
        virtual void init() = 0;

        ref<VertexArray> getVertexArray() const { return vertexArray; }
        ref<VertexBuffer> getVertexBuffer() const { return vertexBuffer; }
        ref<IndexBuffer> getIndexBuffer() const { return indexBuffer; }

    protected:
        /**
         * Once a child class is instantiated and has filled the vertices and
         * indices, it must call this function.
         */
        void createBuffers();

    protected:
        /** OpenGL Vertex Array Object. Used when binding before making a draw
         * call. */
        ref<VertexArray> vertexArray = nullptr;
        /** OpenGL Vertex Buffer Object. Used to manipulate vertex attributes
         * like position. */
        ref<VertexBuffer> vertexBuffer = nullptr;
        /** OpenGL Element Buffer Object. Used to reduce number of required
         * vertex to draw a primitive. */
        ref<IndexBuffer> indexBuffer = nullptr;
        /** Tells OpenGL whether we will change the topology.
         * @details Usually should be set to GL_STATIC_DRAW. Can also be set to
         * GL_DYNAMIC_DRAW.
         */
        GLenum drawType = GL_STATIC_DRAW;

        std::vector<float> vertices;
        std::vector<uint32_t> indices;
    };

    class PrimitiveQuad : public Shape {
    public:
        PrimitiveQuad() = default;
        virtual void init();

    private:
    };

    class PrimitiveCube : public Shape {
    public:
        PrimitiveCube() = default;
        virtual void init();

    private:
    };
}
