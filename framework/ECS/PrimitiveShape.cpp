#include "PrimitiveShape.h"

namespace FW {
    PrimitiveShape::PrimitiveShape(std::vector<float> vertices,
                                   std::vector<uint32_t> indices,
                                   bool instanced)
    {
    }

    void PrimitiveShape::createBuffers()
    {
        drawType = GL_STATIC_DRAW;
//
//        auto i = std::move(indices);
//        auto v = std::move(vertices);
//
//        auto entityAttribLayout = FW::BufferLayout({
//          { FW::ShaderDataType::Float3, "a_position" },
//          { FW::ShaderDataType::Float4, "a_color" },
//          { FW::ShaderDataType::Float2, "a_texCoord" },
//          { FW::ShaderDataType::Float3, "a_normal" },
//        });
//
//        vertexArray = new FW::VertexArray();
//        vertexArray->bind();
//
//        indexBuffer = new FW::IndexBuffer(i.data(), static_cast<int>(i.size()));
//
//        vertexBuffer = new FW::VertexBuffer(
//          v.data(), static_cast<int>(v.size()) * sizeof(float), drawType);
//
//        vertexBuffer->setLayout(entityAttribLayout);
//        vertexArray->setIndexBuffer(indexBuffer);
//        vertexArray->addVertexBuffer(vertexBuffer);
//
//        recalculateModelMatrix();
//
//        isDrawable = true;
//
//        material = Material(MaterialPreset::CUSTOM);
    }
}
