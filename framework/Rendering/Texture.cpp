#include "Texture.h"

#include "assertions.h"

#include <stb_image.h>

#include "Shader.h"

namespace Framework {
	Texture::~Texture()
	{
//        std::cout << "Destructor: Texture [Name: " << name << "]" << std::endl;
		glDeleteTextures(1, &textureID);
	}

    void Texture::createWhiteTexture(Shader* shader, const std::string& name) {
        width = 1;
        height = 1;
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;

        // Upload to OpenGL
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        glTextureStorage2D(textureID, 1, internalFormat, width, height);

        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);
        uint32_t whiteTextureData = 0xffffffff;
        glTextureSubImage2D(
                textureID,
                0,
                0,
                0,
                width,
                height,
                dataFormat,
                GL_UNSIGNED_BYTE,
                (void*)&whiteTextureData
        );

        type = TextureType::WhiteTexture;
        this->name = name;
        textureTarget = GL_TEXTURE_2D;
        this->shader = shader;
    }

    void Texture::loadTexture2D(
            Shader* _shader, const std::string& name, const std::string& filepath, int slot) {

        this->shader = _shader;

        // stbi_image's v-axis in the uv-coordinate space is inverse to ours.
        stbi_set_flip_vertically_on_load(1);
        unsigned char* pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!pixels) {
            framework_assert("Failed to load texture: " + filepath);
        }

        // We only support image formats with RGB or RGBA channels
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } else {
            framework_assert("Failed to upload texture: We only support 8-bit RGB or RGBA formats");
        }

        // Create texture and upload to OpenGL
        textureSlot = slot;
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glTextureStorage2D(textureID, 1, internalFormat, width, height);

        // Wrapping
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(
                textureID,
                0,
                0,
                0,
                width,
                height,
                dataFormat,
                GL_UNSIGNED_BYTE,
                pixels
        );

        stbi_image_free(pixels);
        type = TextureType::Texture2D;
        this->name = name;
        textureTarget = GL_TEXTURE_2D;
    }

    void Texture::loadCubeMap(
            Shader* _shader, const std::string& name_, const std::string& filepath, int slot) {
        this->shader = _shader;
        auto pixels = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        if (!pixels) {
            framework_assert("Failed to load texture: " + filepath);
        }

        // We only support image formats with RGB or RGBA channels
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } else {
            framework_assert("Failed to upload texture: We only support 8-bit RGB or RGBA formats");
        }

        // Create texture and upload to OpenGL
        textureSlot = slot;
        textureTarget = GL_TEXTURE_2D;

        // Create texture ID
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);

        // Bind texture
        glActiveTexture(GL_TEXTURE0 + textureSlot);

        // Allocate memory for texture
        glTextureStorage2D(textureID, 1, internalFormat, width, height);

        //Wrapping
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

        //Filtering
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture to allocated memory
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, pixels);
        }

        stbi_image_free(pixels);
        type = TextureType::CubeMap;
        this->name = name_;
    }

    void Texture::bind()
    {
        int samplerSwitch;

        switch (type) {
            case Texture2D: samplerSwitch = 0; break;
            case CubeMap: samplerSwitch = 1; break;
            case WhiteTexture: samplerSwitch = 0; break;
            default: 0;
        }

        shader->bind();
        shader->setInt("u_samplerSwitch", samplerSwitch);
        glBindTextureUnit(textureSlot, textureID);
        glBindTexture(textureTarget, textureID);
    }

    void Texture::unbind()
    {
        glBindTextureUnit(0, textureID);
    }
}