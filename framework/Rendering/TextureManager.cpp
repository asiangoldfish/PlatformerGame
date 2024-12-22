#include "TextureManager.h"
#include "Log.h"
#include "Shader.h"

namespace FW {
    std::vector<ref<Texture>> TextureManager::textures;
    uint32_t TextureManager::invalidTextureID =
      std::numeric_limits<uint32_t>::max();

    void TextureManager::bind(const std::string& name, int textureSlot) {
        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->name == name) {
                texture->bind(textureSlot);
                return;
            }
            index++;
        }

        textures[0]->bind(textureSlot);
    }

    void TextureManager::bind(uint32_t id, int textureSlot) {
        // Bind to the first texture with the given name.
        int index = 0;
        for (auto& texture : textures) {
            if (texture->textureID == id) {
                texture->bind(textureSlot);
                return;
            }
            index++;
        }

        // The shader might expect a texture, and the first one should be a
        // white texture
        if (!textures.empty()) {
            textures[0]->bind(textureSlot);
        }
        // else {
        //     WARN("TextureManager::bind(): The first texture should be white,
        //     "
        //          "but no texture was found");
        // }
    }

    uint32_t TextureManager::loadTexture2D(const std::string& name,
                                           const std::string& filepath) {
        // If a texture with the same filepath already exists, then don't load
        // it.

        ref<Texture> t = createRef<Texture>();
        t->loadTexture2D(name, filepath);
        textures.push_back(t);
        return t->textureID;
    }

    void TextureManager::loadTexture2D(
      const std::initializer_list<TexturePath>& texturePath) {
        for (const auto& tex : texturePath) {
            loadTexture2D(tex.name, tex.filePath);
        }
    }

    uint32_t TextureManager::loadCubeMap(
      const std::string& name,
      const std::vector<std::string>& filePaths) {
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
                                           glm::vec2 size) {
        ref<Texture> t = createRef<Texture>();
        t->createTexture(name, hexColors, size);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::createTexture(const std::string& name,
                                           glm::vec3 rgbColors,
                                           glm::vec2 size) {
        ref<Texture> t = createRef<Texture>();
        t->createTexture(name, rgbColors, size);
        textures.push_back(t);
        return t->textureID;
    }

    uint32_t TextureManager::getTextureID(const std::string& name) {
        for (const auto& texture : textures) {
            if (texture->name == name) {
                return texture->getTextureId();
            }
        }

        return invalidTextureID;
    }

    std::string TextureManager::getTextureName(const uint32_t id) {
        for (const auto& texture : textures) {
            if (texture->textureID == id) {
                return texture->name;
            }
        }

        return "";
    }

    bool TextureManager::deleteTexture(const std::string& name) {
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

    bool TextureManager::deleteTexture(const int id) {
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

    const std::vector<ref<Texture>>& TextureManager::getTextures() {
        return textures;
    }
} // namespace Framework
