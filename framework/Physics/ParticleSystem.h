#pragma once

#include "pch.h"

// External libraries
#include <glm/glm.hpp>

// Framework
#include "Entity.h"

namespace FW {
    class Particle
    {
    public:
        Particle() = default;
        Particle(glm::vec3 initialVelocity);

        virtual void update(float deltaTime, float gravity);
        virtual void draw(ref<Shader>& shader);

    private:
        friend class Emitter;

        int32_t id = -1;
        glm::vec3 position{ 0.0f };
        float size = 1.0f;
        float lifeTime = 0.0;
        glm::vec3 velocity{ 0.0f };

        // Temporary
        float randomRadius;

        scope<Entity> displayEntity;
    };

    class Emitter
    {
    public:
        Emitter();
        virtual ~Emitter();

        virtual void update(float deltaTime);
        virtual void draw();

        void addParticle(int amount, bool clampMax = false);
        void removeParticle(uint32_t id);

        const ref<Shader>& getShader() { return shader; }

    private:
        void initiateEmitter();
        void recalculateModelMatrix(glm::vec3 translate,
                                    glm::vec3 rotate,
                                    glm::vec3 scale);

    private:
        int32_t id = -1;
        glm::vec3 position{ 0.0f };

        std::vector<ref<Particle>> particlesPool;
        uint32_t numOfParticles = 0;
        uint32_t currentlyReserved = 10000;
        uint32_t reserveIncrement = 10000;
        uint32_t maxParticles = 10;

        // Physics properties
        float gravity = 0.98f;
        glm::vec3 initialVelocity{1.5f};

        ref<Shader> shader;
        VertexArray* vertexArray = nullptr;
        VertexBuffer* vertexBuffer = nullptr;
        IndexBuffer* indexBuffer = nullptr;
        glm::mat4 modelMatrix;
    };
}
