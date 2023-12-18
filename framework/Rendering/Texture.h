#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h>

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

        /** Create a white texture in memory */
        void createWhiteTexture(Shader* shader, const std::string& name);

        /** Load a 2-dimensional texture from disk */
        void loadTexture2D(Shader* _shader,
                           const std::string& name,
                           const std::string& filepath,
                           int slot = 0);

        /** Load a cube map from disk */
        void loadCubeMap(Shader* _shader,
                         const std::string& name,
                         const std::string& filepath,
                         int slot = 0);

        /**
         * Bind the texture
         * @param slot Texture slot to bind
         * TODO: Add support for multiple textures
         * TODO: Make an assertion when an empty texture is bound
         */
        void bind();

        /**
         * Unbind the texture
         * @param slot Texture slot to unbind
         */
        void unbind();

        /**
         * Set the texture's type.
         *
         * This is important to determine what sampler type is used in the
         * fragment shader.
         */
        void setType(TextureType newType) { type = newType; };

    private:
        /** Gives the TextureManager access to Texture's private members */
        friend class TextureManager;

        /** We need the shader to bind the correct texture unit */
        Shader* shader = nullptr;

        /** The texture's name. Acts as an interface to make binding easier */
        std::string name;

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