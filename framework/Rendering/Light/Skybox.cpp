// ----------
// C++ libraries
// ----------
#include <vector>

// ---------
// External libraries
// ---------
#include "glm/gtc/matrix_transform.hpp"

//----------
// Framework
//----------
#include "assertions.h"
#include "Log.h"

// OpenGL
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"

#include "GeometricTools.h"

// Rendering
#include "RenderCommands.h"
#include "Shader.h"
#include "TextureManager.h"

// Entity
#include "Skybox.h"

namespace Framework {
    Skybox::Skybox(int textureId)
    {
        this->textureId = textureId;

        auto entityAttribLayout = Framework::BufferLayout(
          { { Framework::ShaderDataType::Float3, "a_position" } });

        vertexArray = new Framework::VertexArray();
        vertexArray->bind();

        std::vector<float> vertices = GeometricTools::SkyboxGeometryVertices();
        std::vector<uint32_t> indices = GeometricTools::SkyboxGeometryIndices();

        indexBuffer = new IndexBuffer(indices.data(), indices.size());

        vertexBuffer =
          new VertexBuffer(vertices.data(),
                           static_cast<int>(vertices.size() * sizeof(float)),
                           GL_STATIC_DRAW);

        vertexBuffer->setLayout(entityAttribLayout);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->addVertexBuffer(vertexBuffer);

        recalculateModelMatrix();
    }

    // Draw itself and all child nodes.
    void Skybox::draw(Shader& shader)
    {
        shader.bind();

        // We want to change the depth testing before rendering the skybox. By
        // doing this, we save on performance by limiting how many fragments we
        // are testing against.
        auto oldDepthTestingFunc = RenderCommand::getCurrentDepthFunc();
        RenderCommand::setCurrentDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);


        // Upload all required uniforms
        shader.setMat4("u_model", modelMatrix);
        TextureManager::bind(textureId, 0);
        RenderCommand::drawIndex(*vertexArray);

        glCullFace(GL_FRONT);
        glFrontFace(GL_CW);
        glDepthMask(GL_TRUE);
        RenderCommand::setCurrentDepthFunc(oldDepthTestingFunc);
    }

    void Skybox::recalculateModelMatrix()
    {
        modelMatrix = glm::translate(glm::mat4(1.0f), position) *
                      glm::scale(glm::mat4(1.0f), scale);
    }

    Skybox::~Skybox()
    {
        delete vertexArray;
        vertexArray = nullptr;
    }
} // Framework
