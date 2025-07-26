#pragma once

#include "pch.h"

// Framework
#include "Buffer.h"

namespace RenderCommand {
    /**
     * List of supported modes in the fragment shader to render in.
     */
    enum class VisualizeMode
    {
        NORMAL = 0,    // All rendering effects are applied
        DEPTH_TESTING, // Visualize depth testing with a gray scale.
    };

    /** Render mode */
    enum class PolygonMode
    {
        SOLID = 0,
        WIREFRAME,
        POINT
    };

    struct QuadProperties
    {};

    struct RenderingContext
    {
        FW::ref<FW::VertexArray> vertexArray;
        FW::ref<FW::VertexBuffer> vertexBuffer;
        FW::ref<FW::IndexBuffer> indexBuffer;
    };

    static FW::scope<RenderingContext> quadContext = nullptr;

    /**
     * This function must be called before draw calls are performed.
     */
    void init();

    /**
     * This must be called <u>once</u> only during the application's lifetime.
     * @details It is recommended to call this function after the game loop has
     * ended and before <u>glfwTerminate()</u>
     */
    void destroy();

    /**
     * Tell OpenGL what color to put on the screen after clearing it.
     * @param color Background color to render.
     */
    inline void setClearColor(glm::vec3 color)
    {
        glClearColor(color.r, color.g, color.b, 1.0f);
    }

    /**
     * Clear the screen. Should be called per frame.
     * Tip: Use a color like pink. This will help development to find bugs.
     * This works great if we use a sky dome, which should always cover the
     * OpenGL background.
     */
    inline void clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT |
                                    GL_STENCIL_BUFFER_BIT)
    {
        glClear(mode);
    }

    /**
     * Set render modes, like solid or wireframe mode.
     * @param mode Solid, wireframe or point render modes.
     */
    void setPolygonMode(PolygonMode mode);

    /**
     * Draw indexed objects.
     * This only works if the object uses indices (Element Buffer Object).
     * @param vertexArrayObject Vertex Array Object to bind
     * @param primitive OpenGL primitive to draw with. Default: GL_TRIANGLES
     */
    void drawIndex(const FW::VertexArray& vertexArrayObject,
                   GLenum primitive = GL_TRIANGLES);

    inline void drawIndex(const std::shared_ptr<FW::VertexArray>& vao,
                          GLenum primitive = GL_TRIANGLES)
    {
        drawIndex(*vao, primitive);
    }

    /**
     * Temporarily disable or enable depth buffer.
     *
     * Allow performing depth testing on all fragments, but discard them
     * accordingly. This essentially forces OpenGL to use a read-only depth
     * buffer.
     *
     * Note that depth testing must be enabled with glEnable(GL_DEPTH_TEST).
     * @param flag Enabled if set to GL_TRUE. Disabled if set to GL_FALSE
     */
    inline void setDepthMask(GLboolean flag)
    {
        glDepthMask(flag);
    }

    static GLenum currentlyUsedDepthFunc = GL_LESS;

    /**
     * Get the current depth testing function.
     *
     * Available functions:
     * <ul>
     * <li>GL_NEVER</li>
     * <li>GL_ALWAYS</li>
     * <li>GL_LESS</li>
     * <li>GL_EQUAL</li>
     * <li>GL_LEQUAL</li>
     * <li>GL_GREATER</li>
     * <li>GL_NOTEQUAL</li>
     * <li>GL_GEQUAL</li>
     * </ul>
     * @return The currently used function
     */
    inline GLenum getCurrentDepthFunc()
    {
        return currentlyUsedDepthFunc;
    }

    /**
     * Set the current depth testing function.
     *
     * Available functions:
     * <ul>
     * <li>GL_NEVER</li>
     * <li>GL_ALWAYS</li>
     * <li>GL_LESS</li>
     * <li>GL_EQUAL</li>
     * <li>GL_LEQUAL</li>
     * <li>GL_GREATER</li>
     * <li>GL_NOTEQUAL</li>
     * <li>GL_GEQUAL</li>
     * </ul>
     */
    inline void setCurrentDepthFunc(GLenum func)
    {
        currentlyUsedDepthFunc = func;
    }
}
