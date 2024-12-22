#pragma once

#include "pch.h"

// External libraries
#include <glm/glm.hpp>

// Framework
#include "Memory.h"
#include "Buffer.h"
#include "Shader.h"

namespace FW {
    class ParticleShape;

    class Particle
    {
    public:
        /**
         * Construct particle with default parameters.
         */
        Particle() = default;

        /**
         * Construct the particle with an initial velocity.
         * @param initialVelocity The initial velocity when spawning.
         */
        Particle(glm::vec3 initialVelocity);

        // TODO: Move the force to the emitter. The emitter's responsibility is
        // to impact the particle's velocity.
        /**
         * Update the particle over time.
         * @param deltaTime Time between frames.
         * @param gravity Gravity is applied per frame to the velocity.
         */
        virtual void update(float deltaTime, float gravity);

        /**
         * Draw the particle. This is only for visuals, as properties are
         * updated in <u>update()</u>.
         * @param shader The shader to draw with.
         */
        virtual void draw(ref<Shader>& shader);

    private:
        friend class Emitter;

        /** Unique particle identifier. -1 if it has no ID. */
        int32_t id = -1;

        /** Position in world space. */
        glm::vec3 position{ 0.0f };

        // TODO: Change to non-uniform size
        /** Particle uniform size */
        float size = 1.0f;

        /** How long the particle currently has lived */
        float lifetime = 0.0;

        /** If less than 0, the particle never dies */
        float maxLifetime = 4.0f;

        /** Velocity through world space */
        glm::vec3 velocity{ 0.0f };

        /** The shape to display the particle with */
        scope<ParticleShape> displayEntity;

        /** Whether the particle is still alive */
        bool isAlive = true;

        /** The particle's color */
        glm::vec4 color{ 1.0f };
    };

    class Emitter
    {
    public:
        Emitter();
        virtual ~Emitter() = default;

        /**
         * Update the emitter and particles.
         * @details Typical properties that are updated include gravitational
         * acceleration applied to velocity. Velocity is thereafter applied to
         * position.
         * @param deltaTime
         */
        virtual void update(float deltaTime);

        /**
         * Draw particles to screen.
         */
        virtual void draw();

        /**
         * Add <i>n</i> new particles to emit.
         * @details If randomness is applied to certain properties, like max
         * lifetime, then each of them will have their own individual property.
         * @param amount Number of new particles to emit.
         */
        void addParticle(int amount);

        /**
         * Get the shader used to draw the particles.
         */
        const ref<Shader>& getShader() { return shader; }

        /**
         * Set the particles lifetime.
         *
         * @details If the min and max are different, then use a random value
         * between them. This can be set in runtime. The new values are applied
         * to all new particles. Existing particles remain unchanged.
         *
         * @param min Minimum particle lifetime.
         * @param max Maximum particle lifetime.
         */
        void setMaxLifetime(float min, float max)
        {
            maxLifetime = glm::vec2(min, max);
        }

        /**
         * Set the max possible particles.
         *
         * @details Once the limit is reached, no more particles can be spawned.
         * If the max particles is set below the current capacity, then new
         * particles are unable to spawn before the older ones die.
         *
         * @param value The new max particle limit.
         */
        void setMaxParticles(uint32_t value) { maxParticles = value; }

        /**
         * Set the initial velocity in the X-direction.
         *
         * @param min The minimum velocity
         * @param max The maximum velocity
         */
        void setInitialVelocityX(float min, float max)
        {
            initialVelocityX = glm::vec2(min, max);
        }

        /**
         * Set the initial velocity in the Y-direction.
         *
         * @param min The minimum velocity
         * @param max The maximum velocity
         */
        void setInitialVelocityY(float min, float max)
        {
            initialVelocityY = glm::vec2(min, max);
        }

        /**
         * Set the initial velocity in the Z-direction.
         *
         * @param min The minimum velocity
         * @param max The maximum velocity
         */
        void setInitialVelocityZ(float min, float max)
        {
            initialVelocityZ = glm::vec2(min, max);
        }

        /**
         * Set gravitational acceleration applied to particles over time.
         *
         * @details Gravitational acceleration is applied to the particles'
         * velocity each step (default: per
 frame). Please note that although
         * the real-world value is close to 9.8m/s², this does not necessarily
         * apply to computer graphics. Thus, value is not physically accurate.
         *
         * @param value The new gravitational acceleration.
         */
        void setGravity(const float value) { gravity = value; }

    private:
        /**
         * Recalculate a particle's model matrix.
         * @details This should be called before drawing.
         */
        void recalculateModelMatrix(glm::vec3 translate,
                                    glm::vec3 rotate,
                                    glm::vec3 scale);

        /**
         * Find the first available ID
         * @return The new ID to apply to the particle.
         */
        int32_t findAvailableParticleID();

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
        glm::vec2 initialVelocityX{ 0.0f };
        glm::vec2 initialVelocityY{ 0.0f };
        glm::vec2 initialVelocityZ{ 0.0f };

        // ------------
        // Miscellaneous
        // ------------
        ref<Shader> shader;
        ref<ParticleShape> particleShape;
        glm::mat4 modelMatrix;
    };

    class ParticleShape
    {
    public:
        ParticleShape();
        virtual ~ParticleShape() = default;

    private:
        friend class Emitter;
        friend class Particle;
        ref<VertexArray> vertexArray;
        ref<VertexBuffer> vertexBuffer;
        ref<IndexBuffer> indexBuffer;
    };
}
