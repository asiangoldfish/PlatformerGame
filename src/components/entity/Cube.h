#ifndef LAB_CUBE_H
#define LAB_CUBE_H

#include "Framework.h"

class Cube : public Framework::Entity
{
public:
    Cube() = default;
    explicit Cube(Framework::Shader* shader)
    {
        initDrawable(shader, GeometricTools::UnitCubeGeometry3D(), GeometricTools::UnitCubeGeometry3DIndices());
    }

    glm::vec4 getOriginalColor() & { return originalColor; }
    void setOriginalColor(const glm::vec4& color) { originalColor = color; }

    glm::vec3 getOriginalPosition() & { return originalPosition; }
    void setOriginalPosition(const glm::vec3& pos) { originalPosition = pos; }

private:
    glm::vec4 originalColor = glm::vec4(1.0f);
    glm::vec3 originalPosition = glm::vec4(0.0f);
};

#endif // LAB_CUBE_H
