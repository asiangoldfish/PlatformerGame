// ---------
// External libraries
// ---------
#include "glm/gtc/matrix_transform.hpp"

//----------
// Framework
//----------
#include "Log.h"

// OpenGL
#include "Buffer.h"

// Rendering
#include "RenderCommands.h"
#include "Shader.h"
#include "TextureManager.h"

// Entity
#include "Entity.h"

namespace FW {
    // Make the entity drawable
    void Entity::initDrawable(std::vector<float> vertices,
                              std::vector<uint32_t> indices)
    {
        drawType = GL_DYNAMIC_DRAW;

        auto i = std::move(indices);
        auto v = std::move(vertices);

        auto entityAttribLayout = FW::BufferLayout({
          { FW::ShaderDataType::Float3, "a_position" },
          { FW::ShaderDataType::Float4, "a_color" },
          { FW::ShaderDataType::Float2, "a_texCoord" },
          { FW::ShaderDataType::Float3, "a_normal" },
        });

        vertexArray = new FW::VertexArray();
        vertexArray->bind();

        indexBuffer = new FW::IndexBuffer(i.data(), static_cast<int>(i.size()));

        vertexBuffer = new FW::VertexBuffer(
          v.data(), static_cast<int>(v.size()) * sizeof(float), drawType);

        vertexBuffer->setLayout(entityAttribLayout);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->addVertexBuffer(vertexBuffer);

        recalculateModelMatrix();

        isDrawable = true;

        material = Material(MaterialPreset::CUSTOM);
    }

    // Draw itself and all child nodes.
    void Entity::draw(const ref<Shader>& shader)
    {
        if (isDrawable) {
            shader->bind();
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
            FW::TextureManager::bind(material.getProperties().diffuseTextureID,
                                     0);

            // Specular
            /*
            if (material.getProperties().specularTextureId != -1) {
                FW::TextureManager::bind(material.getProperties().specularTextureId);
            } else if (material.getProperties().isSpecularTextureSet()) {
                FW::TextureManager::bind(
                  material.getProperties().specularTextureName);
            } else {
                FW::TextureManager::bind("no-texture-spec");
            }
             */

            // Shininess
            shader->setFloat("u_material.shininess",
                             material.getProperties().shininess);

            RenderCommand::drawIndex(*vertexArray);
        }

        for (const auto& child : children) {
            child->draw(shader);
        }
    }

    // Recursively update itself and child nodes
    void Entity::update()
    {
        if (isDrawable) {
            boundingBox.setPosition(position);
            boundingBox.setScale(scale);
        }

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
