#include "Entity.h"

#include "RenderCommands.h"

#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include "Physics.h"

Entity::Entity(Framework::Shader* shader,
	std::vector<float> vertices,
	std::vector<uint32_t> indices,
	GLenum drawType
)
	: position(0.0f)
	, scale(1.0f)
	, modelMatrix(1.0f)
	, shader(shader)
	, vertices(vertices)
	, indices(indices)
	, drawType(drawType)
{
	init();
}

void Entity::draw() const
{
    // Upload all required uniforms
	shader->setMat4("u_model", modelMatrix);
    shader->setFloat4("u_color", color);

    RenderCommand::drawIndex(*vertexArray);
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

	modelMatrix = glm::translate(modelMatrix, position) *
		newRotation *
		glm::scale(modelMatrix, scale);
}


void Entity::move(glm::vec3 moveBy)
{
	position += moveBy;
    Physics::update(*this);
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

void Entity::setRotation(float yaw, float pitch, float roll) {
	this->yaw = yaw;
	this->pitch = pitch;
	this->roll = roll;
	recalculateModelMatrix();
}

void Entity::init()
{
	// Generate vertices and indices

	auto EntityAttribLayout = Framework::BufferLayout({
	{ Framework::ShaderDataType::Float3, "a_position" },
	{ Framework::ShaderDataType::Float4, "a_color"    },
	{ Framework::ShaderDataType::Float2, "a_texCoord" },
	{ Framework::ShaderDataType::Float3, "a_normal"   },
	});

	vertexArray = new Framework::VertexArray();
	vertexArray->bind();

	indexBuffer = new Framework::IndexBuffer(
		indices.data(), indices.size()
	);

	vertexBuffer = new Framework::VertexBuffer(
		vertices.data(),
		vertices.size() * sizeof(float),
		drawType
	);

	vertexBuffer->setLayout(EntityAttribLayout);
	vertexArray->setIndexBuffer(indexBuffer);
	vertexArray->addVertexBuffer(vertexBuffer);

	recalculateModelMatrix();

    boundingBox = new Physics::BoundingBox();
}

Entity::~Entity() {
    if (vertexArray) {
        delete vertexArray;
        vertexArray = nullptr;
    }

    if (vertexBuffer) {
        delete vertexBuffer;
        vertexBuffer = nullptr;
    }

    if (indexBuffer) {
        delete indexBuffer;
        indexBuffer = nullptr;
    }

    if (boundingBox) {
        delete boundingBox;
        boundingBox = nullptr;
    }
}

void Entity::setColor(const glm::vec4 &color) {
    this->color = color;
}

/**
 * Retrieves positional vertices from the vertices member variable.
 * with anything else.
 */
const std::vector<glm::vec3> Entity::getVertices() {
    const int offset = 0, stride = 12, vertexSize = 3;
    std::vector <glm::vec3> vectorReturnVector;
    glm::vec4 u4;
    glm::vec3 u3;

    // Go through all elements in member vector "vertices".
    // These include position, colour, texture, normals...
    for (int i = 0; i < vertices.size(); i++) {

        // Get positional vertices by indices.
        if (( (i + offset) % stride) == 0) {

            // w = 1.0 here, cause mat multiplication is dot products x_x
            u4 = glm::vec4 (vertices[i+0], vertices[i+1], vertices[i+2], 1.0f);
            u4 = modelMatrix * u4;

            u3 = glm::vec3(u4.x, u4.y, u4.z);

            vectorReturnVector.push_back(u3);
        }
    }

    return vectorReturnVector;
}
