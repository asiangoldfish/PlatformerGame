//
// Created by khai on 25/10/23.
//

#include "TextureManager.h"

#include "Shader.h"

namespace Framework {
    void TextureManager::loadTexture(
            const std::string& name,
            const std::string& filepath,
            TextureManager::TextureFormat format) {

        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        switch (format) {
            case Texture2D:
                t->loadTexture2D(shader, name, filepath);
                break;
            case Texture3D: // TODO: Implement
                break;
            case CubeMap:
                t->loadCubeMap(shader, name, filepath);
                break;
            case SkyBox: // TODO: Implement
                break;
        }

        textures.push_back(t);
    }

    void TextureManager::bind(const std::string &name) {
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
        t->bind();
    }

    void TextureManager::createTexture(const std::string &name) {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->createWhiteTexture(shader, name);
        textures.push_back(t);
    }
} // namespace Framework