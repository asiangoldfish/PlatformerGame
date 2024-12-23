#pragma once

#include "pch.h"

// External
#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader;

/**
 * The Texture class represents a texture object in OpenGL. It should be
 * managed by the TextureManager.
 *
 * @details There are two ways to create a new texture: Either create it
 * programmatically with the <i>createTexture()</i> methods, or with the
 * loadTexture() function variants.
 *
 * @details Please note that each Texture instance manages its own texture
 * object's lifetime on the GPU. This means that when creating a new
 * texture, it is ideally heap allocated and freed accordingly. Consider
 * using smart pointers for better memory safety.
 *
 * @details Info about the OpenGL texture: <a
 * href="https://www.khronos.org/opengl/wiki/Texture">The Khronos Group
 * Wiki</a>
 *
 * @example
 * @code
 * Texture* loadedTexture = new Texture();
 * loadedTexture->loadTexture2D("Example texture",
 * "textures/example_texture.jpg");
 *
 * Texture* createdTexture = new Texture();
 * loadedTexture->createTexture("Example texture", 0xFFFFFF, { 1.0f, 1.0f
 * });
 * @endcode
 *
 * @details Once the texture is loaded, it can now be bound. Binding a
 * non-existing texture will yield a warning.
 * @details <b><i>Format support:</i></b><br>
 * Currently, the Texture only supports RGB and RGBA 8-bit channels.
 *
 */
class Texture {
public:
    /**
     * The texture's format.
     */
    enum TextureType { Texture2D = 0, CubeMap, WhiteTexture };

public:
    /**
     * Create a new texture and set a uniform colour by hexadecimal values.
     *
     * @param name The texture's name. Used to identify it when binding.
     * @param hexColor The uniform color to fill the texture with.
     * @param size The texture's width and height.
     * @return Texture ID that also can be used to identify the texture
     * with.
     */
    uint32_t createTexture(const std::string& name_,
                           uint32_t hexColor = 0xFFFFFF,
                           glm::vec2 size = glm::vec2(1.0f));

    /**
     * Create a new texture and set a uniform colour by hexadecimal values.
     *
     * @param name The texture's name. Used to identify it when binding.
     * @param color The uniform color to fill the texture with.
     * @param size The texture's width and height.
     * @return Texture ID that also can be used to identify the texture
     * with.
     */
    uint32_t createTexture(const std::string& name,
                           glm::vec3 color = glm::vec3(0.0f),
                           glm::vec2 size = glm::vec2(1.0f));

    /**
     * Load a texture from disk.
     *
     * @details The texture is internally stored as a two-dimensional
     * texture. The primary usage for this texture is mapping data to
     * geometry surfaces.<br>
     * In the GLSL shader, the texture is sampled with <i>sampler2D</i>.
     *
     * @param name The texture's name. This is used when binding with the
     * TextureManager.
     * @param filepath Filepath to load the texture from disk.
     */
    uint32_t loadTexture2D(const std::string& name,
                           const std::string& filepath);

    /**
     * Load a cube map from disk.
     *
     * @detail A cube map primary usage is to map textures on each of
     * a cube's surfaces. Instead of using texture coordinates, it uses
     * the cube's world position to project the textures onto each of
     * the cube's face.<br>
     * In the GLSL shader, the cube map is sampled with <i>samplerCube<i>
     *
     * @param name The cube map's name. This us used when binding with
     * the TextureManager.
     * @param filePath File path to the texture on disk. This one texture
     * is projected for all the cube's faces.
     */
    uint32_t loadCubeMap(const std::string& name, const std::string& filePath);

    /**
     * Load a cube map from disk.
     *
     * @detail A cube map primary usage is to map textures on each of a
     * cube's surfaces. Instead of using texture coordinates, it uses
     * the cube's world position to project the textures onto each of
     * the cube's face.<br>
     * In the GLSL shader, the cube map is sampled with <i>samplerCube<i>
     *
     * @param name The cube map's name. This us used when binding with
     * the TextureManager.
     * @param filePaths File paths to the texture on disk. The order
     * of the paths matters and is FIFO.
     */
    uint32_t loadCubeMap(const std::string& name,
                         const std::vector<std::string>& filePaths);

    /**
     * Bind the texture.
     *
     * @details To bind to a unit, pass the desire slot number as
     * <i>textureSlot</i>.
     *
     * @param textureSlot The texture slot or unit to bind to. Internally,
     * <i>glActiveTexture(GL_TEXTURE0 + i)</i> is called.
     *
     * @warning If the texture slot is less than 0, a warning is printed on
     * console.
     */
    void bind(int textureSlot) const;

    /** Get the texture's file path on disk. */
    [[nodiscard]] const std::string& getFilepath() const { return filepath; }

    /** Get the texture's ID */
    [[nodiscard]] uint32_t getTextureId() const { return textureID; }

    /** Get the texture's name */
    [[nodiscard]] const std::string& getName() const { return name; }

    /**
     * Construct Texture.
     *
     * This constructs an empty Texture class. You should load or create a
     * texture before binding it.
     */
    Texture() = default;

    /**
     * Free GPU resources.
     *
     * @details Each instance manages its own texture object. By calling the
     * destructor, this instance's texture object is deallocated correctly.
     */
    virtual ~Texture();

private:
    /** Gives the TextureManager access to Texture's private members */
    friend class TextureManager;

    /** The texture's name. Acts as an interface to make binding easier */
    std::string name;

private:
    /** Filepath that the texture is located at on disk */
    std::string filepath;

    /** The texture's ID. This is used whenever binding it */
    uint32_t textureID = 0;

    /** The texture's type. */
    TextureType type = TextureType::WhiteTexture;

    /** Specifies the target when binding the texture. */
    GLenum textureTarget = 0;
};
