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
#include "Log.h"

// OpenGL
#include "Buffers/VertexArray.h"
#include "Buffers/VertexBuffer.h"
#include "Buffers/IndexBuffer.h"

// Rendering
#include "RenderCommands.h"
#include "Shader.h"
#include "TextureManager.h"

// Entity
#include "Entity.h"

namespace Framework {
    // Make the entity drawable
    void Entity::initDrawable(Framework::Shader* shader,
                              std::vector<float> vertices,
                              std::vector<uint32_t> indices)
    {
        this->shader = shader;
        drawType = GL_DYNAMIC_DRAW;

        auto i = std::move(indices);
        auto v = std::move(vertices);

        auto entityAttribLayout = Framework::BufferLayout({
          { Framework::ShaderDataType::Float3, "a_position" },
          { Framework::ShaderDataType::Float4, "a_color" },
          { Framework::ShaderDataType::Float2, "a_texCoord" },
          { Framework::ShaderDataType::Float3, "a_normal" },
        });

        vertexArray = new Framework::VertexArray();
        vertexArray->bind();

        indexBuffer =
          new Framework::IndexBuffer(i.data(), i.size());

        vertexBuffer = new Framework::VertexBuffer(
          v.data(), v.size() * sizeof(float), drawType);

        vertexBuffer->setLayout(entityAttribLayout);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->addVertexBuffer(vertexBuffer);

        recalculateModelMatrix();

        isDrawable = true;

        material = Material(MaterialPreset::CUSTOM);
    }

    // Draw itself and all child nodes.
    void Entity::draw()
    {
        if (isDrawable) {
            shader->bind();
            // Upload all required uniforms
            shader->setMat4("u_model", modelMatrix);
            shader->setFloat4("u_color", color);

            // Upload material properties
            //        shader->setFloat3("u_material.ambient",
            //        material.getProperties().ambient);
            //        shader->setFloat3("u_material.diffuse",
            //        material.getProperties().diffuse);

            // We allow the user to either upload a texture or set it by value
            // -----
            // Diffuse
            /*
            if (material.getProperties().diffuseTextureId != -1) {
                Framework::TextureManager::bind(material.getProperties().diffuseTextureId);
            } else if (material.getProperties().isDiffuseTextureSet()) {
                Framework::TextureManager::bind(
                  material.getProperties().diffuseTextureName);
            } else {
                Framework::TextureManager::bind("no-texture-diff");
            }
            // Specular
            if (material.getProperties().specularTextureId != -1) {
                Framework::TextureManager::bind(material.getProperties().specularTextureId);
            } else if (material.getProperties().isSpecularTextureSet()) {
                Framework::TextureManager::bind(
                  material.getProperties().specularTextureName);
            } else {
                Framework::TextureManager::bind("no-texture-spec");
            }
             */

            // Shininess
            shader->setFloat("u_material.shininess",
                             material.getProperties().shininess);

            RenderCommand::drawIndex(*vertexArray);
        }

        for (const auto& child : children) {
            child->draw();
        }
    }

    // Recursively update itself and child nodes
    void Entity::update()
    {
        for (const auto& child : children) {
            child->update();
        }
    }

    void Entity::recalculateModelMatrix()
    {
        modelMatrix = glm::mat4(1.0f);
        // Translation

        glm::mat4 newRotation, rotx, roty, rotz;

        // Rotation
        rotx =
          glm::rotate(modelMatrix, glm::radians(yaw), { 1.0f, 0.0f, 0.0f });
        roty =
          glm::rotate(modelMatrix, glm::radians(pitch), { 0.0f, 1.0f, 0.0f });
        rotz =
          glm::rotate(modelMatrix, glm::radians(roll), { 0.0f, 0.0f, 1.0f });

        newRotation = rotx * roty * rotz;

        modelMatrix = glm::translate(modelMatrix, position) * newRotation *
                      glm::scale(modelMatrix, scale);
    }

    void Entity::move(glm::vec3 moveBy)
    {
        position += moveBy;
        recalculateModelMatrix();
    }

    void Entity::rotate(float yawBy, float pitchBy, float rollBy)
    {
        yaw += yawBy;
        pitch += pitchBy;
        roll += rollBy;
        recalculateModelMatrix();
    }

    void Entity::setRotation(glm::vec3 rotation)
    {
        setRotation(rotation.x, rotation.y, rotation.z);
    }

    void Entity::setRotation(float yaw, float pitch, float roll)
    {
        this->yaw = yaw;
        this->pitch = pitch;
        this->roll = roll;
        recalculateModelMatrix();
    }

    Entity::~Entity()
    {
        delete vertexArray;
        vertexArray = nullptr;

        for (auto& child : children) {
            delete child;
        }
    }

    void Entity::setColor(const glm::vec4& color)
    {
        this->color = color;
    }

    void Entity::setColor(const glm::vec3& color)
    {
        this->color = glm::vec4(color, 1.0f);
    }

    void Entity::setColor(const float color)
    {
        this->color = glm::vec4(color, color, color, 1.0f);
    }

    /**
     * Retrieves positional vertices from the vertices member variable.
     * with anything else.
     */
    const std::vector<glm::vec3> Entity::getVertices()
    {
        const int offset = 0, stride = 12, vertexSize = 3;
        std::vector<glm::vec3> vectorReturnVector;
        glm::vec4 u4;
        glm::vec3 u3;

        // Go through all elements in member vector "vertices".
        // These include position, colour, texture, normals...
        for (int i = 0; i < vertices.size(); i++) {

            // Get positional vertices by indices.
            if (((i + offset) % stride) == 0) {

                // w = 1.0 here, cause mat multiplication is dot products x_x
                u4 = glm::vec4(
                  vertices[i + 0], vertices[i + 1], vertices[i + 2], 1.0f);
                u4 = modelMatrix * u4;

                u3 = glm::vec3(u4.x, u4.y, u4.z);

                vectorReturnVector.push_back(u3);
            }
        }

        return vectorReturnVector;
    }
} // Framework
