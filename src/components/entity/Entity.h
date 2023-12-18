#ifndef LAB_ENTITY_H
#define LAB_ENTITY_H

#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"

#include <glm/glm.hpp>

#include <cstdint>
#include <memory>

namespace Physics {
    class BoundingBox;
}

class Entity
{
public:
    Entity() = default;
    Entity(
        Framework::Shader* shader,
        std::vector<float> vertices, 
        std::vector<uint32_t> indices,
    	GLenum drawType = GL_DYNAMIC_DRAW
    );

    virtual ~Entity();

    virtual void draw() const;

    /**
     * Move the entity by a certain distance from it's current position
     * @param moveBy How much to move by
     */
    void move(glm::vec3 moveBy);

    /**
     * Rotate by some degrees
     */
    void rotate(float yawBy, float pitchBy, float rollBy);

    void setRotation(glm::vec3 rotation);
    void setRotation(float yaw, float pitch, float roll);

    inline void setScale(const float scale)
    {
        this->scale.x = scale;
        this->scale.y = scale;
        this->scale.z = scale;
        recalculateModelMatrix();
    }
    inline void setScale(const glm::vec3& scale) { this->scale = scale; }

    inline const glm::vec3& getScale() const { return scale; }

    inline void setPosition(glm::vec3 position)
    {
        this->position = position;
        recalculateModelMatrix();
    }
    inline const glm::vec3& getPosition() const
    {
        return position;
    }

    inline const glm::vec4& getColor() const { return color; }
    void setColor(const glm::vec4& color);

    const std::vector<glm::vec3> getVertices();

    // Physics
    Physics::BoundingBox* getBoundingBox() { return boundingBox; }

private:
    void init();
    void recalculateModelMatrix();

protected:
    Framework::VertexArray* vertexArray = nullptr;
    Framework::VertexBuffer* vertexBuffer = nullptr;
    Framework::IndexBuffer* indexBuffer = nullptr;

    Framework::Shader* shader = nullptr;

    // Transformation
    glm::mat4 modelMatrix;
    glm::vec3 position;
    float yaw = 0.0f,
        pitch = 0.0f,
        roll = 0.0f;

    // This color is multiplied by texture color
    glm::vec4 color{1.0f, 1.0f, 1.0f, 1.0f};

    std::vector<float> vertices;
    std::vector<uint32_t> indices;

    glm::vec3 scale;

    GLenum drawType;

    // Disable per-entity-basis
    bool enableDepthTesting = true;

    // Physics
    Physics::BoundingBox* boundingBox = nullptr;
};

#endif // LAB_ENTITY_H