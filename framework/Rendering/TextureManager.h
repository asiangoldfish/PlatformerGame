#pragma once

#include "pch.h"

#include <limits>

// External
#include <glm/glm.hpp>

// Framework
#include "Texture.h"

class Shader;

/**
 * Container for Texture objects.
 *
 * @details <h1>Overview</h1>The TextureManager acts as a datastructure or
 * container and gives global access to the application's textures. Because
 * it is static, there is only one <i>TextureManager</i> in the application.
 *
 * @details <h1>Memory Management</h1>
 * While textures must be created or loaded by the user, they will
 * automatically be destroyed by the TextureManager's destructor. Call the
 * <u>clearTextures()</u> function to synchronously delete all textures.
 * Please note that this is <b>NOT</b> thread safe.
 *
 * @details <h1>Usage</h1>
 * To load a new texture from disk, use one of the <u>load*()</u> methods.
 * To create a new texture programmatically, use one of the <u>create*()</u>
 * methods. <br> Before a texture can be used, it must be bound. The
 * <i>TextureManager</i> supports binding by name or ID. A texture slot must
 * also be specified while binding. This determines which active texture
 * unit should be used.
 *
 * @example
 * @code
 * // To load a new texture
 * uint32_t fooID = TextureManager::loadTexture2D("foo", "bar/foo.jpg");
 *
 * // To create a new texture
 * uint32_t foo2ID = TextureManager::createTexture("foo2", 0xFFFFFF,
 * glm::vec2(1.0f, 1.0f));
 *
 * // Bind texture by ID
 * TextureManager::bind("foo", 0);
 *
 * // Bind texture by name
 * TextureManager::bind(foo2ID, 0);
 * @endcode
 */
class TextureManager {
public:
    struct TexturePath {
        TexturePath(std::string name, std::string filePath)
          : name(name)
          , filePath(filePath) {}

        std::string name;
        std::string filePath;
    };

public:
    /**
     * Load a texture from disk.
     *
     * @param name The texture's name. This name is used to refer to the
     * texture when binding it.
     * @param filepath File path to the texture on disk.
     */
    static uint32_t loadTexture2D(const std::string& name,
                                  const std::string& filepath);

    /**
     * Load a list of textures from disk.
     *
     * @details The syntax for this method goes as follows:
     * @code
     * TextureManager::loadTexture2D({
     *     { "name_1", "filepath_1" },
     *     { "name_2", "filepath_2" },
     * });
     * @endcode
     *
     * @param texturePath List of texture paths to load.
     */
    static void loadTexture2D(
      const std::initializer_list<TexturePath>& texturePath);

    /**
     * Load a textures from file.
     *
     * @details The textures are sampled as cube map by OpenGL. Provide 6
     * file paths to sample them for each cube face.
     *
     * @param name The texture's name. This name is used to refer to the
     * texture when binding it.
     * @param filepath File path to the texture on disk.
     */
    static uint32_t loadCubeMap(const std::string& name,
                                const std::vector<std::string>& filePaths);

    /**
     * Create a new texture in memory.
     *
     * @details The texture is created with a uniform color. Applying
     * different colors for each pixel is currently not supported yet.
     *
     * @param name The texture's name. This is used when binding.
     * @param hexColors The texture's uniform color. It is specified with
     * hexadecimals (ex. 0xFFFFFF or 0xffffff).
     * @param size The texture's resolution.
     * @return Texture ID. This can be used while binding, as an alternative
     * to its name.
     */
    static uint32_t createTexture(const std::string& name,
                                  uint32_t hexColors,
                                  glm::vec2 size);

    /**
     * Create a new texture in memory.
     *
     * @details The texture is created with a uniform color. Applying
     * different colors for each pixel is currently not supported yet.
     *
     * @param name The texture's name. This is used when binding.
     * @param rgbColors The texture's uniform color. It is specified with
     * hexadecimals (ex. glm::vec3(1.0f, 1.0f, 1.0f).
     * @param size The texture's resolution.
     * @return Texture ID. This can be used while binding, as an alternative
     * to its name.
     */
    static uint32_t createTexture(const std::string& name,
                                  glm::vec3 rgbColors,
                                  glm::vec2 size);

    /**
     * Get the texture's ID by name.
     * @param name The texture's name
     * @return If the texture exists, then its ID is returned. Otherwise
     * <u>invalidTextureID</u> is returned.
     */
    static uint32_t getTextureID(const std::string& name);

    static std::string getTextureName(const uint32_t id);

    /**
     * Bind the texture by its name.
     *
     * @details If the texture is not found, an INFO message is printed but
     * nothing will happen.
     *
     * @param name The texture's name
     * @param textureSlot Active texture slot to bind to.
     */
    static void bind(const std::string& name, int textureSlot);

    /**
     * Bind the texture by its ID.
     *
     * @details If the texture is not found, an INFO message is printed but
     * nothing will happen.
     *
     * @param id The texture's ID
     * @param textureSlot Active texture slot to bind to.
     */
    static void bind(uint32_t id, int textureSlot);

    /**
     * Delete and clear all textures.
     *
     * @details All textures' memory will be deallocated. New textures can
     * be loaded or created, or the GLFW application can safely be
     * terminated.
     */
    static void clearTextures() { textures.clear(); }

    /**
     * Delete a texture.
     *
     * @param name The name of the texture to match for deletion.
     * @return If the deletion is successful, return true. Otherwise return
     * false.
     */
    static bool deleteTexture(const std::string& name);

    /**
     * Delete a texture.
     *
     * @param id The ID of the texture to match for deletion.
     * @return If the deletion is successful, return true. Otherwise return
     * false.
     */
    static bool deleteTexture(const int id);

    /**
     * Get all textures.
     *
     * @details For memory safety, <b>DO NOT</b> perform memory related
     * commands like deallocations. Instead, use <u>clear*()</u> methods for
     * this purpose. Non-memory related commands are safe to perform.
     * @return All textures currently loaded in memory.
     */
    static const std::vector<std::shared_ptr<Texture>>& getTextures();

    static const int getInvalidTextureID() { return invalidTextureID; }

private:
    // TODO: textures should be a map. This forces their name to be unique.
    static std::vector<std::shared_ptr<Texture>> textures;

    static uint32_t invalidTextureID;
};
