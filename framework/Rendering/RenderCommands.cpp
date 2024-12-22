// External libraries
#include <glad/glad.h>
#include <glm/glm.hpp>

// Framework
#include "RenderCommands.h"
#include "GeometricTools.h"
#include "Log.h"

namespace RenderCommand {
    void init()
    {
        // RenderCommand must only be initialized once
        if (quadContext) {
            FATAL("RenderCommand::init: QuadContext not NULL. RenderCommand "
                  "must only be initialized once");
        } else {
            quadContext = FW::createScope<RenderingContext>();
        }

        auto entityAttribLayout = FW::BufferLayout({
          { FW::ShaderDataType::Float3, "a_position" },
          { FW::ShaderDataType::Float4, "a_color" },
          { FW::ShaderDataType::Float2, "a_texCoord" },
          { FW::ShaderDataType::Float3, "a_normal" },
        });

        auto vertices = FW::UnitGridGeometry2D();
        auto indices = FW::UnitGridIndices2D;

        quadContext->vertexArray = FW::createRef<FW::VertexArray>();
        quadContext->vertexArray->bind();

        quadContext->indexBuffer = FW::createRef<FW::IndexBuffer>(&indices.front(), indices.size());

        quadContext->vertexBuffer = FW::createRef<FW::VertexBuffer>(&vertices.front(),
                                               vertices.size() * sizeof(float));

        quadContext->vertexBuffer->setLayout(entityAttribLayout);
        quadContext->vertexArray->setIndexBuffer(quadContext->indexBuffer);
        quadContext->vertexArray->addVertexBuffer(quadContext->vertexBuffer);
    }

    void destroy()
    {
        quadContext = nullptr;
    }

    void setPolygonMode(PolygonMode mode)
    {
        GLuint polyMode;
        switch (mode) {
            case PolygonMode::SOLID:
                polyMode = GL_FILL;
                break;
            case PolygonMode::WIREFRAME:
                polyMode = GL_LINE;
                break;
            case PolygonMode::POINT:
                polyMode = GL_POINT;
                break;
            default:
                polyMode = 0x00;
        }

        // Face should only be GL_FRONT_AND_BACK
        glPolygonMode(GL_FRONT_AND_BACK, polyMode);
    }

    void drawIndex(const FW::VertexArray& vertexArrayObject, GLenum primitive)
    {
        uint32_t count = vertexArrayObject.getIndexBuffer()->getCount();

        vertexArrayObject.bind();
        glDrawElements(GL_TRIANGLES, (int)count, GL_UNSIGNED_INT, nullptr);
    }
}
