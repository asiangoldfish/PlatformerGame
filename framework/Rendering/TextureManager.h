#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

// C++ libraries
#include <memory>
#include <vector>

// External
#include <glm/glm.hpp>

// Framework
#include "Texture.h"

class Shader;

namespace Framework {
    /**
     * Container for Texture objects.
     *
     * The TextureManager acts as a data structure or container and gives global
     * access to the application's textures. Because it is static, there is
     * only one TextureManager in the application.
     */
    class TextureManager
    {
    public:
        /**
         * Currently supported types: Texture2D and CubeMap.
         */
        enum TextureFormat
        {
            Texture2D = 0,
            Texture3D,
            CubeMap,
            SkyBox,
            WhiteTexture
        };

    public:
        /**
         * Load a texture from file.
         *
         * Notes about texture slot: This is used to apply multiple textures at
         * the same time. The following slots are used for the following
         * purposes:
         * - Slot 1: Diffuse texture (base colour)
         * - Slot2: Specular texture (reflectivity)
         *
         * If you only need colours, use slot 1.
         *
         * @param name The texture's name. This name is used to refer to the
         * texture when binding it.
         * @param filepath File path to the texture on disk.
         * @param format Texture format. See TextureFormat documentation
         * supported formats.
         * @param textureSlot The slot to put the texture in.
         */
        static int loadTexture(const std::string& name,
                               const std::string& filepath,
                               TextureFormat format,
                               int textureSlot,
                               bool invertPixels = false);

        static int loadCubemap(Shader* shader_,
                               const std::string& name,
                               std::vector<std::string> filepaths,
                               TextureFormat format,
                               int textureSlot);

        static int createTexture(const std::string& name,
                                 uint32_t hexColors,
                                 glm::vec2 size,
                                 int textureSlot = 0);
        static int createTexture(const std::string& name,
                                 glm::vec3 rgbColors,
                                 glm::vec2 size,
                                 int textureSlot = 0);

        /** Bind a texture by name */
        static void bind(const std::string& name, Shader* shader = nullptr);

        /** Bind a texture by id */
        static void bind(int id, Shader* shader = nullptr);

        static void setShader(Shader* s) { shader = s; }

        static void clearTextures() { textures.clear(); }

        static const std::vector<std::shared_ptr<Texture>>& getTextures();

    private:
        // TODO: textures should be a map. This forces their name to be unique.
        static std::vector<std::shared_ptr<Texture>> textures;
        static Shader* shader;
    };

} // Framework

#endif // TEXTURE_MANAGER_H
