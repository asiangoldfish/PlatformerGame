#ifndef TEXTURE_H
#define TEXTURE_H

// C++ libraries
#include <string>
#include <vector>

// External
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Framework {
    class Shader;

    /**
     * The Texture class represents a texture object in OpenGL. It should be
     * stored in TextureManager.
     */
    class Texture
    {
    public:
        /** The texture's format. */
        enum TextureType
        {
            Texture2D = 0,
            CubeMap,
            WhiteTexture
        };

    public:
        /**
         * Construct Texture.
         * This constructs an empty Texture class. You should load or create a
         * texture before binding it.
         */
        Texture() = default;
        virtual ~Texture();

        /**
         * Create a new texture and set a uniform colour by hexadecimal values
         * to it
         * @param shader The shader used to upload this texture to
         * @param name The texture's name. Used to identify it when binding.
         * @param textureSlot Texture slot to assign it.
         */
        uint32_t createTexture(Shader* shader,
                           const std::string& name,
                           uint32_t color,
                           glm::vec2 size = glm::vec2(1.0f),
                           int slot = 0);

        /**
         * Create a texture and set a uniform colour by RGB values to it.
         *
         * @param shader The shader used to upload this texture to
         * @param name The texture's name. Used to identify it when binding.
         * @param color The colour values that the texture slot contain
         * @param textureSlot Texture slot to assign it.
         */
        uint32_t createTexture(Shader* shader,
                           const std::string& name,
                           glm::vec3 color,
                           glm::vec2 size = glm::vec2(1.0f),
                           int slot = 0);

        /** Load a 2-dimensional texture from disk */
        uint32_t loadTexture2D(Shader* _shader,
                           const std::string& name,
                           const std::string& filepath,
                           int slot = 0);

        /** Load a cube map from disk */
        uint32_t loadCubeMap(Shader* _shader,
                         const std::string& name,
                         const std::string& filepath,
                         int slot = 0);

        uint32_t loadCubeMap(Shader* _shader,
                         const std::string& name,
                         const std::vector<std::string>& filepaths,
                         int slot = 0);
        /**
         * Bind the texture
         * @param slot Texture slot to bind
         * TODO: Add support for multiple textures
         * TODO: Make an assertion when an empty texture is bound
         */
        void bind(Shader* shader);

        /**
         * Unbind the texture
         * @param slot Texture slot to unbind
         */
        //        void unbind();

        /**
         * Set the texture's type.
         *
         * This is important to determine what sampler type is used in the
         * fragment shader.
         */
        void setType(TextureType newType) { type = newType; };

        /** Get the texture's file path on disk */
        [[nodiscard]] const std::string& getFilepath() const
        {
            return filepath;
        }

        [[nodiscard]] uint32_t getTextureId() const { return textureID; }
        [[nodiscard]] const std::string& getName() const { return name; }

    private:
        /** Gives the TextureManager access to Texture's private members */
        friend class TextureManager;

        /** We need the shader to bind the correct texture unit */
        Shader* shader = nullptr;

        /** The texture's name. Acts as an interface to make binding easier */
        std::string name;

    private:
        /** Filepath that the texture is located at on disk */
        std::string filepath;

        /** The texture's ID. This is used whenever binding it */
        uint32_t textureID;

        /** The texture's type. */
        TextureType type;

        /** The texture's size */
        int width, height;

        /** The texture's channels. This is fetched from the texture file */
        int channels;

        /** Texture slot that this texture object is bound to */
        int textureSlot = 0;

        int32_t internalFormat;
        GLenum dataFormat;
        GLenum textureTarget; // Used when binding
    };
}

#endif // TEXTURE_H
