#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "Texture.h"

#include <memory>
#include <vector>

class Shader;

namespace Framework {
    /**
     * Container for Texture objects.
     *
     * The TextureManager acts as a data structure or container and gives global
     * access to the application's textures. Because it is static, there is
     * only one TextureManager in the application.
     */
    class TextureManager {
    public:
        /**
         * Currently supported types: Texture2D and CubeMap.
         */
        enum TextureFormat { Texture2D = 0, Texture3D, CubeMap, SkyBox, WhiteTexture };

    public:
        /**
         * Load a texture from file.
         * @param name The texture's name. This name is used to refer to the
         * texture when binding it.
         * @param filepath File path to the texture on disk.
         * @param format Texture format. See TextureFormat documentation
         * supported formats.
         */
        void loadTexture(
                const std::string& name,
                const std::string& filepath,
                TextureFormat format);

        void createTexture(const std::string& name);

        /** Bind a texture by name */
        void bind(const std::string& name);

        void setShader(Shader* s) { shader = s; }

    private:
        // TODO: textures should be a map. This forces their name to be unique.
        std::vector<std::shared_ptr<Texture>> textures;
        Shader* shader = nullptr;
    };

} // Framework

#endif // TEXTURE_MANAGER_H
