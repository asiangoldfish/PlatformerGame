#pragma once

#include "pch.h"

#include "Shape.h"
#include "Shader.h"
#include "Material.h"
#include "Memory.h"

namespace FW {

    class Component {
    public:
        Component() = default;
        Component(std::string componentName)
          : name(componentName) {}
        virtual ~Component() = default;

        /** Initialise the component. */
        virtual void init() = 0;

        /** Update the component. Should be called once per frame. */
        virtual void update(float delta) = 0;

    public:
        std::string name;
    };

    /**
     * The drawable component is required to render the Entity to the screen.
     *
     * It is important to initialise the drawable's OpenGL components before
     * drawing by calling the setShape() method.
     *
     * Currently, only primitive shapes are supported.
     */
    class DrawableComponent : public Component {
    public:
        DrawableComponent() = default;
        DrawableComponent(std::string componentName)
          : Component(componentName) {}

        virtual void init() override;
        virtual void update(float delta) override;

        void setShape(ref<Shape> shape) { this->shape = shape; }
        void setShader(ref<Shader> shader) { this->shader = shader; }

    private:
        void draw();

    private:
        ref<Shape> shape;
        ref<Shader> shader;
        Material material;
        GLenum drawType;
    };

    class TransformationComponent : public Component {
    public:
        TransformationComponent() = default;
        TransformationComponent(std::string componentName)
          : Component(componentName) {}

        virtual void init() override;
        virtual void update(float delta) override;
        void setShader(ref<Shader> shader) { this->shader = shader; }

        void setPosition(glm::vec3 position);
        glm::vec3 getPosition() { return position; }

        void setScale(float x, float y, float z);
        glm::vec3 getScale() { return scale; }

    private:
        // TODO find some way to only recalculate model matrix once per frame.
        void recalculateModelMatrix();

    private:
        // Transformation
        glm::mat4 modelMatrix;
        ref<Shader> shader;
        glm::vec3 position;
        float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
        glm::vec3 scale = glm::vec3{ 1.0f };
    };

    /**
     * The physics component brings physics capability to an `Entity`.
     *
     * This class is tightly coupled with the Framework's Physics module. It
     * acts as an adapter and enables entities to have physics properties.
     */
    class PhysicsComponent : public Component {
    public:
        virtual void init() override {};
        virtual void update(float delta) override;

        /**
         * Update the current velocity with new values. To add velocity, see
         * PhysicsComponent::addVelocity(...).
         */
        void setVelocity(float x, float y, float z);
        glm::vec3& getVelocity() { return velocity; }
        void addVelocity(float x, float y, float z);
        void addVelocity(float x, float y);

    private:
        float gravity = 9.8067f;
        glm::vec3 velocity{ 0.0f };
    };

} // namespace FW