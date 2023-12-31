#include "ParticleSystem.h"

#include "GeometricTools.h"
#include "FW_Math.h"

namespace FW {
#pragma region Particle
    Particle::Particle(glm::vec3 initialVelocity)
    {
        randomRadius = rng(0.25, 2.5f);
        velocity = initialVelocity;
    }

    void Particle::update(float deltaTime, float gravity)
    {
        lifeTime += deltaTime;

        float radius = 1.5f + lifeTime * 0.25f * randomRadius;
        float speed = lifeTime * 4.0f;

        velocity += gravity;

        position.x = std::cos(speed) * radius;
        position.y += velocity.y * 0.001f;
        position.z = std::sin(speed) * radius;
    }

    void Particle::draw(ref<Shader>& shader)
    {
        displayEntity->draw(shader);
    }
#pragma endregion

#pragma region Emitter
    void Emitter::update(float deltaTime)
    {
        for (auto i = 0; i < numOfParticles; i++) {
            auto particle = particlesPool[i];
            particle->update(deltaTime, gravity);
        }
    }

    void Emitter::draw()
    {
        for (auto i = 0; i < numOfParticles; i++) {
            shader->bind();
            recalculateModelMatrix(particlesPool[i]->position,
                                   glm::vec3(0.0f),
                                   glm::vec3(particlesPool[i]->size));
            shader->setMat4("u_model", modelMatrix);
            RenderCommand::drawIndex(*vertexArray);
            // TODO: Upload u_projection and u_view
        }
    }

    void Emitter::addParticle(int amount, bool clampMax)
    {
        /* TODO: Add mechanism to reserve space and reuse particles
        // If clampMax is enabled, then add x amount of particles, but do not
        // overfill the pool
        if (clampMax && numOfParticles < maxParticles) {
            uint32_t maxSpawnableParticles = maxParticles - numOfParticles;
            while (numOfParticles < maxSpawnableParticles &&
                   numOfParticles < amount) {
                // Increase the pool's reserved space if we're about to go
                // beyond the current limit
                if (++numOfParticles > particlesPool.size()) {
                    particlesPool.reserve(currentlyReserved + reserveIncrement);
                }
            }
        }

        // Only add particles if it doesn't overflow the pool
        if (numOfParticles + amount <= maxParticles) {
            while (++numOfParticles > amount)
                ;
        }
         */

        // TODO: Add clampMax mechanism
        for (int i = 0; i < amount; i++) {
            ref<Particle> particle = createRef<Particle>(initialVelocity);
            particle->size = 0.25f;
            particlesPool.push_back(particle);
            numOfParticles++;
        }
    }

    void Emitter::removeParticle(uint32_t particleId)
    {
        particlesPool.erase(particlesPool.begin() + particleId);
    }

    void Emitter::initiateEmitter()
    {
        auto entityAttribLayout = FW::BufferLayout({
          { FW::ShaderDataType::Float3, "a_position" },
          { FW::ShaderDataType::Float4, "a_color" },
          { FW::ShaderDataType::Float2, "a_texCoord" },
          { FW::ShaderDataType::Float3, "a_normal" },
        });

        auto vertices = UnitCubeGeometry3D();
        auto indices = UnitCubeGeometry3DIndices();

        vertexArray = new VertexArray();
        vertexArray->bind();

        indexBuffer = new IndexBuffer(&indices.front(), indices.size());

        vertexBuffer =
          new VertexBuffer(&vertices.front(), vertices.size() * sizeof(float));

        vertexBuffer->setLayout(entityAttribLayout);
        vertexArray->setIndexBuffer(indexBuffer);
        vertexArray->addVertexBuffer(vertexBuffer);

        shader = FW::createRef<FW::Shader>(
          FW_PHYSICS_RESOURCES_DIR + std::string("shaders/particleVertex.glsl"),
          FW_PHYSICS_RESOURCES_DIR + std::string("shaders/particleFrag.glsl"));
    }

    Emitter::Emitter()
    {
        initiateEmitter();
    }

    Emitter::~Emitter()
    {
        delete vertexArray;
        vertexArray = nullptr;
        delete vertexBuffer;
        vertexBuffer = nullptr;
        delete indexBuffer;
        indexBuffer = nullptr;
    }
    void Emitter::recalculateModelMatrix(glm::vec3 translate,
                                         glm::vec3 rotate,
                                         glm::vec3 scale)
    {
        modelMatrix = glm::mat4(1.0f);

        glm::mat4 newRotation, rotx, roty, rotz;

        // Rotation
        rotx = glm::rotate(
          modelMatrix, glm::radians(rotate.x), { 1.0f, 0.0f, 0.0f });
        roty = glm::rotate(
          modelMatrix, glm::radians(rotate.y), { 0.0f, 1.0f, 0.0f });
        rotz = glm::rotate(
          modelMatrix, glm::radians(rotate.z), { 0.0f, 0.0f, 1.0f });

        newRotation = rotx * roty * rotz;

        modelMatrix = glm::translate(glm::mat4(1.0f), translate) * newRotation *
                      glm::scale(glm::mat4(1.04), scale);
    }
#pragma endregion
}
