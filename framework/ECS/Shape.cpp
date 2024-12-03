#include "Shape.h"
#include "Buffer.h"
#include "GeometricTools.h"

namespace FW {
    void Shape::createBuffers() {
        drawType = GL_DYNAMIC_DRAW;

        auto entityAttribLayout = BufferLayout({
          { ShaderDataType::Float3, "a_position" },
          { ShaderDataType::Float4, "a_color" },
          { ShaderDataType::Float2, "a_texCoord" },
          { ShaderDataType::Float3, "a_normal" },
        });

        vertexArray = createRef<VertexArray>();
        vertexArray->bind();

        indexBuffer = createRef<IndexBuffer>(
          indices.data(), static_cast<int64_t>(indices.size()));

        vertexBuffer = createRef<VertexBuffer>(
          vertices.data(),
          static_cast<int>(vertices.size()) * sizeof(float),
          drawType);

        vertexBuffer->setLayout(entityAttribLayout);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->addVertexBuffer(vertexBuffer);
    }

    void PrimitiveCube::init() {
        indices = UnitCubeGeometry3DIndices();
        vertices = UnitCubeGeometry3D();

        createBuffers();
    }

    void PrimitiveQuad::init() {
        // vertices = UnitGridGeometry2D();
        vertices = UnitSpriteVertices;
        indices = UnitGridIndices2D;

        createBuffers();
    }
}
