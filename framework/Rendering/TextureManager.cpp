//
// Created by khai on 25/10/23.
//

#include "TextureManager.h"

#include "Shader.h"

namespace Framework {
    Shader* TextureManager::shader = nullptr;
    std::vector<std::shared_ptr<Texture>> TextureManager::textures;

    void TextureManager::bind(const std::string& name)
    {
        std::shared_ptr<Texture> t = nullptr;

        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->name == name) {
                t = texture;
                break;
            }
            index++;
        }
        if (t) {
            t->bind();
        }
    }

    void TextureManager::bind(int id)
    {
        std::shared_ptr<Texture> t = nullptr;

        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->textureID == id) {
                t = texture;
                break;
            }
            index++;
        }
        if (t) {
            t->bind();
        }
    }

    int TextureManager::loadTexture(const std::string& name,
                                    const std::string& filepath,
                                    TextureFormat format,
                                    int textureSlot,
                                    bool invertPixels)
    {
        // If a texture with the same filepath already exists, then don't load
        // it.

        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        switch (format) {
            case Texture2D:
                t->loadTexture2D(shader, name, filepath, textureSlot);
                t->setType(Texture::TextureType::Texture2D);
                break;
            case Texture3D: // TODO: Implement
                break;
            case CubeMap:
                t->loadCubeMap(shader, name, filepath, textureSlot);
                t->setType(Texture::TextureType::CubeMap);
                break;
            case SkyBox: // TODO: Implement
                break;
            default:
                break;
        }

        textures.push_back(t);

        return t->textureID;
    }

    int TextureManager::createTexture(const std::string& name,
                                      uint32_t hexColors,
                                      glm::vec2 size,
                                      int textureSlot)
    {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->createTexture(shader, name, hexColors, size, textureSlot);
        t->setType(Texture::TextureType::WhiteTexture);
        textures.push_back(t);

        return static_cast<int>(t->textureID);
    }

    int TextureManager::createTexture(const std::string& name,
                                      glm::vec3 rgbColors,
                                      glm::vec2 size,
                                      int textureSlot)
    {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->createTexture(shader, name, rgbColors, size, textureSlot);
        t->setType(Texture::TextureType::WhiteTexture);
        textures.push_back(t);

        return static_cast<int>(t->textureID);
    }
    const std::vector<std::shared_ptr<Texture>>& TextureManager::getTextures()
    {
        return textures;
    }
} // namespace Framework
