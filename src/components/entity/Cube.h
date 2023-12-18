#ifndef LAB_CUBE_H
#define LAB_CUBE_H

#include "Entity.h"
#include "GeometricTools.h"

#include <vector>

class Cube : public Entity
{
public:
    Cube() = default;
	Cube(Framework::Shader* shader,
		std::vector<float> vertices,
		std::vector<uint32_t> indices
	)
		: Entity(shader, vertices, indices)
	{
	}

    virtual ~Cube() {
//        std::cout << "Destructor: Cube" << std::endl;
    };

    glm::vec4 getOriginalColor()& { return originalColor; }
    void setOriginalColor(const glm::vec4& color) { originalColor = color; }

    glm::vec3 getOriginalPosition()& { return originalPosition; }
    void setOriginalPosition(const glm::vec3& pos) { originalPosition = pos; }

private:
    glm::vec4 originalColor;
    glm::vec3 originalPosition;
};

#endif // LAB_CUBE_H