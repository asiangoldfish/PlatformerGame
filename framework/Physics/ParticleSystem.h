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
        float lifetime = 0.0;

        /** If less than 0, the particle never dies */
        float maxLifetime = 4.0f;
        glm::vec3 velocity{ 0.0f };

        // Temporary
        float randomRadius;

        scope<Entity> displayEntity;

        bool isAlive = true;
    };

    class Emitter
    {
    public:
        Emitter();
        virtual ~Emitter();

        virtual void update(float deltaTime);
        virtual void draw();

        void addParticle(int amount, bool clampMax = false);

        const ref<Shader>& getShader() { return shader; }

        /**
         * Set the particles lifetime.
         * @details If the min and max are different, then use a random value
         * between them. This can be set in runtime. The new values are applied
         * to all new particles. Existing particles remain unchanged.
         * @param min Minimum particle lifetime.
         * @param max Maximum particle lifetime.
         */
        void setmaxLifetime(float min, float max)
        {
            maxLifetime = glm::vec2(min, max);
        }

    private:
        void initiateEmitter();
        void recalculateModelMatrix(glm::vec3 translate,
                                    glm::vec3 rotate,
                                    glm::vec3 scale);
        int32_t findAvailableParticleID();
        void removeParticle(uint32_t id);

    private:
        // ------------
        // Emitter properties
        // ------------
        uint32_t id = -1;
        glm::vec3 position{ 0.0f };

        // ------------
        // Particle properties
        // These are passed down to particles
        // ------------
        /** Particle minimum and maximum lifetime */
        glm::vec2 maxLifetime{ 1.0f };

        // ------------
        // Data related to spawning particles
        // ------------
        std::vector<ref<Particle>> particlesPool;
        uint32_t numOfParticles = 0;
        uint32_t currentlyReserved = 10000;
        uint32_t reserveIncrement = 10000;
        uint32_t maxParticles = 10;

        // ------------
        // Physics properties
        // ------------
        float gravity = 0.98f;
        glm::vec3 initialVelocity{ 1.5f };

        // ------------
        // Miscellaneous
        // ------------
        ref<Shader> shader;
        VertexArray* vertexArray = nullptr;
        VertexBuffer* vertexBuffer = nullptr;
        IndexBuffer* indexBuffer = nullptr;
        glm::mat4 modelMatrix;
    };
}
