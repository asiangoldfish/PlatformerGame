// C++ libraries
#include <cstdint>

// External
#include <stb_image.h>

// Framework
#include "Texture.h"
#include "assertions.h"
#include "Shader.h"
#include "Log.h"

/** Convert RGB values to hexadecimals */
static uint32_t
rgbToHex(const glm::vec3& color)
{
    // Clamp color values to the range [0.0, 1.0]
    glm::vec3 clampedColor = glm::clamp(color, 0.0f, 1.0f);

    // Convert to 8-bit unsigned integer values [0, 255]
    auto red = static_cast<uint8_t>(clampedColor.r * 255);
    auto green = static_cast<uint8_t>(clampedColor.g * 255);
    auto blue = static_cast<uint8_t>(clampedColor.b * 255);

    // Pack the RGB values into a 32-bit hex value
    return (static_cast<uint32_t>(red) << 16) |
           (static_cast<uint32_t>(green) << 8) | static_cast<uint32_t>(blue);
}

namespace FW {
    Texture::~Texture()
    {
        glDeleteTextures(1, &textureID);
    }

    uint32_t Texture::createTexture(const std::string& name_,
                                    uint32_t hexColor,
                                    glm::vec2 size)
    {
        // Set member variables
        type = TextureType::WhiteTexture;
        this->name = name_;
        textureTarget = GL_TEXTURE_2D;

        // Clamp size to at least 1
        size.x = size.x < 1.0f ? 1.0f : size.x;
        size.y = size.y < 1.0f ? 1.0f : size.y;

        // Set the texture's size
        int width = static_cast<int>(size.x);
        int height = static_cast<int>(size.y);

        // Formats used to tell OpenGL what format to store the pixels in
        GLenum internalFormat, dataFormat;
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;

        // Reserve some memory on the GPU
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        glTextureStorage2D(textureID, 1, internalFormat, width, height);

        // Set texture parameters
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Data is read in the inverse order. So before uploading to the gpu, we
        // must invert the order.
        uint32_t textureData = hexColor;
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

        return textureID;
    }

    uint32_t Texture::createTexture(const std::string& name_,
                                    glm::vec3 color,
                                    glm::vec2 size)
    {
        return createTexture(name_, rgbToHex(color), size);
    }

    uint32_t Texture::loadTexture2D(const std::string& name_,
                                    const std::string& filepath_)
    {
        // Set member variables
        filepath = filepath_;
        name = name_;
        type = TextureType::Texture2D;
        textureTarget = GL_TEXTURE_2D;

        int width, height, channels;

        // stbi_image's y-axis in the uv-coordinate space is inverse to ours.
        stbi_set_flip_vertically_on_load(1);
        unsigned char* pixels =
          stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        // Failed to load the image. The program will crash.
        if (!pixels) {
            framework_assert("Failed to load texture: " + filepath);
        }

        // We only support image formats with RGB or RGBA channels
        GLenum internalFormat, dataFormat;
        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat = GL_RGB;
        } else {
            std::string msg = "Failed to load texture \'" + name +
                              "\', at path \'" + filepath + "\'. It contains " +
                              std::to_string(channels) +
                              " channels. We only supports 3 and 4 channels.";
            framework_assert(msg);
        }

        // Allocate space to the texture
        glCreateTextures(GL_TEXTURE_2D, 1, &textureID);
        glTextureStorage2D(textureID, 1, internalFormat, width, height);

        // Set texture parameters
        glTextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        glTextureParameteri(textureID, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(textureID, GL_TEXTURE_WRAP_T, GL_REPEAT);

        // Upload the texture to the GPU
        glTextureSubImage2D(textureID,
                            0,
                            0,
                            0,
                            width,
                            height,
                            dataFormat,
                            GL_UNSIGNED_BYTE,
                            pixels);

        // Free image data. They are no longer needed.
        stbi_image_free(pixels);

        return textureID;
    }

    uint32_t Texture::loadCubeMap(const std::string& name_,
                                  const std::string& filepath_)
    {
        std::vector<std::string> filePaths = {
            filepath_, filepath_, filepath_, filepath_, filepath_, filepath_
        };
        return loadCubeMap(name_, filePaths);
    };

    uint32_t Texture::loadCubeMap(const std::string& name_,
                                  const std::vector<std::string>& filePaths)
    {
        // Set member variables
        type = TextureType::CubeMap;
        textureTarget = GL_TEXTURE_CUBE_MAP;
        name = name_;

        // Allocate data to the texture
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

        // Iterate over all file paths
        for (uint32_t i = 0; i < filePaths.size(); i++) {
            int width, height, nrChannels;

            // Load image data
            unsigned char* data =
              stbi_load(filePaths[i].c_str(), &width, &height, &nrChannels, 0);

            // Upload data to the GPU
            if (data) {
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
                             0,
                             GL_RGB,
                             width,
                             height,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             data);
            } else {
                framework_assert(
                  std::string("CubeMap failed ton load at path \'") +
                  filePaths[i] + "\'");
            }
            stbi_image_free(data);
        }

        // Set texture parameters
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(
          GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(
          GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(
          GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

        return textureID;
    }

    void Texture::bind(int textureSlot) const
    {
        if (textureSlot < 0) {
            WARN("Texture::bind: textureSlot < 0. It must be >= 1.");
            return;
        }

        glActiveTexture(GL_TEXTURE0 + textureSlot);
        glBindTexture(textureTarget, textureID);
    }
}
