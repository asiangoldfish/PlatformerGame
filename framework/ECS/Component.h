#pragma once

#include "pch.h"

#include "Shape.h"
#include "Shader.h"
#include "Material.h"


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

    private:
        void recalculateModelMatrix();

    private:
        // Transformation
        glm::mat4 modelMatrix;
        ref<Shader> shader;
        glm::vec3 position;
        float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
        glm::vec3 scale = glm::vec3{ 1.0f };
    };

    class PhysicsComponent : public Component {
    public:
        virtual void init() override;
        virtual void update(float delta) override;
    };

} // namespace FW