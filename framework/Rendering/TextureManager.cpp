//
// Created by khai on 25/10/23.
//

#include "TextureManager.h"
#include "Log.h"
#include "Shader.h"

namespace Framework {
    std::vector<ref<Texture>> TextureManager::textures;

    void TextureManager::bind(const std::string& name,
                              int textureSlot)
    {
        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->name == name) {
                texture->bind(textureSlot);
                return;
            }
            index++;
        }

        INFO("TextureManager::bind: Texture \'{}\' does not exist", name);
    }

    void TextureManager::bind(uint32_t id, int textureSlot)
    {
        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->textureID == id) {
                texture->bind(textureSlot);
                return;
            }
            index++;
        }

        INFO("TextureManager::bind: Texture by ID {} does not exist", id);
    }

    uint32_t TextureManager::loadTexture2D(const std::string& name,
                                           const std::string& filepath)
    {
        // If a texture with the same filepath already exists, then don't load
        // it.

        ref<Texture> t = createRef<Texture>();
        t->loadTexture2D(name, filepath);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::loadCubeMap(
      const std::string& name,
      const std::vector<std::string>& filePaths)
    {
        // Verify the vector's size == 6
        if (filePaths.size() != 6) {
            WARN("TextureManager::loadCubeMap: Failed to load texture \'{}\'. "
                 "Provided {} file paths, but 6 is required.",
                 name,
                 filePaths.size());
            return 0;
        }

        ref<Texture> t = createRef<Texture>();
        t->loadCubeMap(name, filePaths);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::createTexture(const std::string& name,
                                           uint32_t hexColors,
                                           glm::vec2 size)
    {
        ref<Texture> t = createRef<Texture>();
        t->createTexture(name, hexColors, size);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::createTexture(const std::string& name,
                                           glm::vec3 rgbColors,
                                           glm::vec2 size)
    {
        ref<Texture> t = createRef<Texture>();
        t->createTexture(name, rgbColors, size);
        textures.push_back(t);
        return t->textureID;
    }

    bool TextureManager::deleteTexture(const std::string& name)
    {
        for (int i = 0; i < textures.size(); i++) {
            if (textures[i]->name == name) {
                // Name matches and texture was found
                textures.erase(textures.begin() + i);
                return true;
            }
        }

        // No texture matches the name
        return false;
    }

    bool TextureManager::deleteTexture(const int id)
    {
        for (int i = 0; i < textures.size(); i++) {
            if (textures[i]->textureID == id) {
                // Name matches and texture was found
                textures.erase(textures.begin() + i);
                return true;
            }
        }

        // No texture matches the name
        return false;
    }

    const std::vector<ref<Texture>>& TextureManager::getTextures()
    {
        return textures;
    }
} // namespace Framework
