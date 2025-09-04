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
        DrawableComponent()
          : Component("DrawableComponent") {}
        DrawableComponent(std::string componentName)
          : Component(componentName) {}

        virtual void init() override;
        virtual void update(float delta) override;

        void setShape(ref<Shape> shape) { this->shape = shape; }
        void setShader(std::string shader) { this->shader = shader; }
        std::string getShader() { return shader; }

        /** Create a new texture.
         *
         * This method creates new texture resources. The \param name "name" is
         * the key to reference it using the TextureManager.
         *
         * To use an existing resource instead, pass its ID to this method
         * instead.
         *
         * @param name The name to reference the texture with.
         * @param filepath Absolute or relative path to the texture resource.
         */
        void setTexture(const std::string& name, const std::string& filepath);

        /**
         * Apply an existing texture to the \ref Sprite "Sprite".
         *
         * @param id The texture by ID to apply. Use the \ref TextureManager
         * "TextureManager" to retrieve the ID.
         */
        void setTexture(const uint32_t id);

        void draw();

        /**
         * Set a parameter to be uploaded to the shader.
         *
         * Parameters are uploaded every frame, and the \ref name "name" is the
         * same name used by the shader's respective uniform.
         *
         * This method is normally used together with a custom shader. To create
         * such shader, use the \ref ShaderManager "ShaderManager".
         *
         * @param name the name which the shader will reference.
         *
         * @see ShaderManager
         */
        void setShaderParam(const std::string& name, const UniformType& value);

        /**
         * Remove a shader parameter that is uploaded every frame.
         *
         * @param name the name of the shader to remove
         */
        void removeShaderParam(const std::string& name);

        /**
         * Get all shader parameters.
         */
        const std::unordered_map<std::string, UniformType>& getShaderParams()
          const {
            return shaderParams;
        }

    public:
        glm::vec4 color{ 1.0f };
        uint32_t Z_index = 0;
        bool isTransparent = false;

    private:
        ref<Shape> shape;
        std::string shader;
        Material material;
        GLenum drawType;

        std::unordered_map<std::string, UniformType> shaderParams;
    };

    class TransformationComponent : public Component {
    public:
        TransformationComponent()
          : Component("TransformationComponent") {}
        TransformationComponent(std::string componentName)
          : Component(componentName) {}

        virtual void init() override;
        virtual void update(float delta) override;
        void setShader(const std::string& shader) { this->shader = shader; }

        void setPosition(glm::vec3 position);
        void setPosition(glm::vec2 position);
        void setPosition(float x, float y, float z);
        void setPosition(float x, float y);
        glm::vec3 getPosition() { return position; }
        glm::vec2 getPosition2D() { return glm::vec2(position); }

        void setScale(float x, float y, float z);
        glm::vec3 getScale() { return scale; }
        glm::vec2 getScale2D() { return glm::vec2(scale); }

        void setRotation(float yaw, float pitch, float roll) {
            this->yaw = yaw;
            this->pitch = pitch;
            this->roll = roll;
        }
        glm::vec3 getRotation() { return rotation; }

        /** Recalculate and upload the transformation matrix to the GPU. */
        void uploadTransformationMatrix();

    private:
        // TODO find some way to only recalculate model matrix once per frame.
        void recalculateModelMatrix();

    private:
        // Transformation
        glm::mat4 modelMatrix;
        std::string shader;
        glm::vec3 position{ 0.0f };
        float yaw = 0.0f, pitch = 0.0f, roll = 0.0f;
        glm::vec3 scale = glm::vec3{ 1.0f };
        glm::vec3 rotation = glm::vec3{ 0.0f };
    };

    /**
     * The physics component brings physics capability to an `Entity`.
     *
     * This class is tightly coupled with the Framework's Physics module. It
     * acts as an adapter and enables entities to have physics properties.
     */
    class PhysicsComponent : public Component {
    public:
        PhysicsComponent()
          : Component("PhysicsComponent") {}
        PhysicsComponent(std::string name)
          : Component(name) {}

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