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
        // Lifetime
        lifetime += deltaTime;
        if (maxLifetime > 0 && lifetime > maxLifetime) {
            isAlive = false;
            return;
        }

        // Apply gravity (aka. downward acceleration)
        velocity.y -= gravity;
        position.x += velocity.x;
        position.y += velocity.y + gravity;
        position.z += velocity.z;

        // Color fades as it dies out. Alpha color is based on percentage of
        // life remaining.
        if (maxLifetime >= 0) {
            // Prevent zero-division error
            color.a = 1.0f - lifetime / maxLifetime;
        }
    }

    void Particle::draw(ref<Shader>& shader)
    {
        shader->setFloat4("u_color", color);
        //        displayEntity->draw(shader);
    }
#pragma endregion

#pragma region Emitter
    void Emitter::update(float deltaTime)
    {
        for (const auto& particle : particlesPool) {
            particle->update(deltaTime, gravity);
        }

        // Remove dead particles
        std::erase_if(particlesPool, [&](const ref<Particle>& p) {
            if (!p->isAlive) {
                numOfParticles--;
                return true;
            }
            return false;
        });
    }

    void Emitter::draw()
    {
        for (auto& particle : particlesPool) {
            particle->draw(shader);

            shader->bind();
            recalculateModelMatrix(
              particle->position, glm::vec3(0.0f), glm::vec3(particle->size));
            shader->setMat4("u_model", modelMatrix);
            RenderCommand::drawIndex(*vertexArray);
        }
    }

    void Emitter::addParticle(int amount)
    {
        // TODO: Add mechanism to reserve space and reuse particles
        for (int i = 0; i < amount && numOfParticles < maxParticles; i++) {
            // Determine the particle's initial velocity by randomness
            glm::vec3 newInitialVelocity = glm::vec3(
              // X-value
              rng(initialVelocityX.x, initialVelocityX.y),
              // Y-value
              rng(initialVelocityY.x, initialVelocityY.y),
              // Z-value;
              rng(initialVelocityZ.x, initialVelocityZ.y));

            ref<Particle> particle = createRef<Particle>(newInitialVelocity);
            particle->size = 0.25f;
            particle->id = findAvailableParticleID();
            particle->position = position;

            // Set particle lifetime
            particle->maxLifetime = maxLifetime.x == maxLifetime.y
                                      ? maxLifetime.x
                                      : rng(maxLifetime.x, maxLifetime.y);

            // Temporary: Add random color
            particle->color = glm::vec4(
              rng(0.0f, 1.0f), rng(0.0f, 1.0f), rng(0.0f, 1.0f), 1.0f);

            particlesPool.push_back(particle);
            numOfParticles++;
        }
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

    int32_t Emitter::findAvailableParticleID()
    {
        // Sort particles pool
        std::sort(particlesPool.begin(),
                  particlesPool.end(),
                  [](const ref<Particle>& p1, ref<Particle> p2) {
                      return p1->id < p2->id;
                  });

        // No particles in this emitter, so return ID equals 0
        if (particlesPool.empty()) {
            return 0;
        }

        // Particles exist. Find the first available id
        for (int32_t i = 0; i < particlesPool.size(); i++) {
            // Particle ID is sorted. If there is a gap in the ID sequence,
            // return the first one
            if (particlesPool[i]->id != i) {
                return i;
            }
        }

        // When the code reaches this part, it means that all particle IDs are
        // chronological. We can therefore return the last ID added by one.
        return static_cast<int32_t>(particlesPool.size());
    }
#pragma endregion
}
