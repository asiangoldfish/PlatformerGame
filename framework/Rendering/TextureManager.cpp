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
        t->bind();
    }

    void TextureManager::loadTexture(const std::string& name,
                                     const std::string& filepath,
                                     TextureFormat format)
    {

        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        switch (format) {
            case Texture2D:
                t->loadTexture2D(shader, name, filepath);
                t->setType(Texture::TextureType::Texture2D);
                break;
            case Texture3D: // TODO: Implement
                break;
            case CubeMap:
                t->loadCubeMap(shader, name, filepath);
                t->setType(Texture::TextureType::CubeMap);
                break;
            case SkyBox: // TODO: Implement
                break;
        }

        textures.push_back(t);
    }

    void TextureManager::createTexture(const std::string& name)
    {
        std::shared_ptr<Texture> t = std::make_shared<Texture>();
        t->createWhiteTexture(shader, name);
        t->setType(Texture::TextureType::WhiteTexture);
        textures.push_back(t);
    }
} // namespace Framework