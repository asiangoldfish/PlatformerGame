// C++ libraries
#include <cstdint>

// External
#include <stb_image.h>

// Framework
#include "Texture.h"
#include "assertions.h"
#include "Shader.h"
#include "Log.h"

namespace Framework {

    uint32_t rgbToHex(const glm::vec3& color)
    {
        // Clamp color values to the range [0.0, 1.0]
        glm::vec3 clampedColor = glm::clamp(color, 0.0f, 1.0f);

        // Convert to 8-bit unsigned integer values [0, 255]
        auto red = static_cast<uint8_t>(clampedColor.r * 255);
        auto green = static_cast<uint8_t>(clampedColor.g * 255);
        auto blue = static_cast<uint8_t>(clampedColor.b * 255);

        // Pack the RGB values into a 32-bit hex value
        return (static_cast<uint32_t>(red) << 16) |
               (static_cast<uint32_t>(green) << 8) |
               static_cast<uint32_t>(blue);
    }

    Texture::~Texture()
    {
        //        std::cout << "Destructor: Texture [Name: " << name << "]" <<
        //        std::endl;
        glDeleteTextures(1, &textureID);
    }

    uint32_t Texture::createTexture(Shader* shader,
                                const std::string& name,
                                uint32_t color,
                                glm::vec2 size,
                                int slot)
    {
        // Clamp size to at least 1
        size.x = size.x < 1.0f ? 1.0f : size.x;
        size.y = size.y < 1.0f ? 1.0f : size.y;

        width = static_cast<int>(size.x);
        height = static_cast<int>(size.y);
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;

        this->textureSlot = slot;

        // Upload to OpenGL
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        glTextureStorage2D(textureID, 1, internalFormat, width, height);

        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Data is read in the inverse order. So before uploading to the gpu, we
        // must invert the order.
        uint32_t textureData = color;
        uint32_t invertedData = ((textureData & 0xff) << 16) |
                                (textureData & 0xff00) |
                                ((textureData & 0xff0000) >> 16);

        // Upload to the GPU
        glTextureSubImage2D(textureID,
                            0,
                            0,
                            0,
                            width,
                            height,
                            dataFormat,
                            GL_UNSIGNED_BYTE,
                            (void*)&invertedData);

        type = TextureType::WhiteTexture;
        this->name = name;
        textureTarget = GL_TEXTURE_2D;
        this->shader = shader;

        return textureID;
    }

    uint32_t Texture::createTexture(Shader* shader,
                                const std::string& name,
                                glm::vec3 color,
                                glm::vec2 size,
                                int slot)
    {
        return createTexture(shader, name, rgbToHex(color), size, slot);
    }

    uint32_t Texture::loadTexture2D(Shader* _shader,
                                const std::string& name,
                                const std::string& filepath,
                                int slot)
    {

        this->shader = _shader;
        textureSlot = slot;
        this->filepath = filepath;

        // stbi_image's v-axis in the uv-coordinate space is inverse to ours.
        stbi_set_flip_vertically_on_load(1);
        unsigned char* pixels =
          stbi_load(this->filepath.c_str(), &width, &height, &channels, 0);

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
            framework_assert("Failed to upload texture: We only support 8-bit "
                             "RGB or RGBA formats");
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

        glTextureSubImage2D(textureID,
                            0,
                            0,
                            0,
                            width,
                            height,
                            dataFormat,
                            GL_UNSIGNED_BYTE,
                            pixels);

        stbi_image_free(pixels);
        type = TextureType::Texture2D;
        this->name = name;
        textureTarget = GL_TEXTURE_2D;

        return textureID;
    }

    uint32_t Texture::loadCubeMap(Shader* _shader,
                              const std::string& name_,
                              const std::string& filepath,
                              int slot)
    {
        this->shader = _shader;
        textureSlot = slot;
        this->filepath = filepath;

        auto pixels =
          stbi_load(this->filepath.c_str(), &width, &height, &channels, 0);

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
            framework_assert("Failed to upload texture: We only support 8-bit "
                             "RGB or RGBA formats");
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

        // Wrapping
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);

        // Filtering
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Set texture to allocated memory
        for (unsigned int i = 0; i < 6; i++) {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                         0,
                         internalFormat,
                         width,
                         height,
                         0,
                         dataFormat,
                         GL_UNSIGNED_BYTE,
                         pixels);
        }

        stbi_image_free(pixels);
        type = TextureType::CubeMap;
        this->name = name_;

        return textureID;
    }

    void Texture::bind(Shader* shader)
    {
        int samplerSwitch;

        switch (type) {
            case Texture2D:
                samplerSwitch = 0;
                break;
            case CubeMap:
                samplerSwitch = 1;
                break;
            case WhiteTexture:
                samplerSwitch = 0;
                break;
            default:
                samplerSwitch = 0;
        }

        shader->bind();

        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(textureTarget, textureID);
    }

    uint32_t Texture::loadCubeMap(Shader* _shader,
                              const std::string& name,
                              const std::vector<std::string>& filepaths,
                              int slot)
    {
        textureSlot = slot;
        this->shader = shader;

        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        for (uint32_t i = 0; i < filepaths.size(); i++) {
            int width, heigth, nrChannels;

            unsigned char* data = stbi_load(filepaths[i].c_str(), &width, &height, &nrChannels, 0);

            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            } else {
                WARN("CubeMap failed ton load at path '{}'", filepaths[i]);
            }
            stbi_image_free(data);
        }

        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
                        GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
                        GL_CLAMP_TO_EDGE);

        glActiveTexture(GL_TEXTURE0);
        type = TextureType::CubeMap;
        textureTarget = GL_TEXTURE_CUBE_MAP;
        return textureID;
    }
}
