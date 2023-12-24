//
// Created by khai on 25/10/23.
//

#include "TextureManager.h"
#include "Log.h"
#include "Shader.h"

namespace Framework {
    std::vector<std::shared_ptr<Texture>> TextureManager::textures;

    void TextureManager::bind(const Shader& shader, const std::string& name, int textureSlot)
    {
        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->name == name) {
                texture->bind(shader, textureSlot);
                return;
            }
            index++;
        }
    }

    void TextureManager::bind(const Shader& shader, int id, int textureSlot)
    {
        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->textureID == id) {
                texture->bind(shader, textureSlot);
                return;
            }
            index++;
        }
    }

    uint32_t TextureManager::loadTexture2D(const std::string& name,
                                      const std::string& filepath)
    {
        // If a texture with the same filepath already exists, then don't load
        // it.

        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->loadTexture2D(name, filepath);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::loadCubeMap(const std::string& name,
                                    const std::vector<std::string>& filePaths)
    {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->loadCubeMap(name, filePaths);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::createTexture(const std::string& name,
                                      uint32_t hexColors,
                                      glm::vec2 size)
    {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->createTexture(name, hexColors, size);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::createTexture(const std::string& name,
                                      glm::vec3 rgbColors,
                                      glm::vec2 size)
    {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->createTexture(name, rgbColors, size);
        textures.push_back(t);
        return t->textureID;
    }
    const std::vector<std::shared_ptr<Texture>>& TextureManager::getTextures()
    {
        return textures;
    }
} // namespace Framework
