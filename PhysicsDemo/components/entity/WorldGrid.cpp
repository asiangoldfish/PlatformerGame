#include "WorldGrid.h"

WorldGrid::WorldGrid() {
    auto entityAttribLayout = FW::BufferLayout({
      { FW::ShaderDataType::Float3, "a_position" }
    });

    auto vertices = FW::UnitGridGeometry2D();
    auto indices = FW::UnitGridIndices2D;

    vertexArray = FW::createRef<FW::VertexArray>();
    vertexArray->bind();

    indexBuffer = FW::createRef<FW::IndexBuffer>(&indices.front(), indices.size());

    vertexBuffer = FW::createRef<FW::VertexBuffer>(
      &vertices.front(), vertices.size() * sizeof(float));

    vertexBuffer->setLayout(entityAttribLayout);
    vertexArray->setIndexBuffer(indexBuffer);
    vertexArray->addVertexBuffer(vertexBuffer);
}
void
WorldGrid::draw(const FW::ref<FW::Shader> shader)
{
    shader->bind();
    RenderCommand::drawIndex(*vertexArray);
}
