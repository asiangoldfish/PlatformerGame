/**
 * Header only file that contains rendering commands
 */

#ifndef RENDER_COMMANDS_H
#define RENDER_COMMANDS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>
#include "VertexArray.h"

namespace RenderCommand
{
    /** Render mode */
	enum class PolygonMode {
		SOLID = 0, WIREFRAME, POINT
	};

    /**
     * Tell OpenGL what color to put on the screen after clearing it.
     * @param color Background color to render.
     */
	inline void setClearColor(glm::vec3 color) {
		glClearColor(color.r, color.g, color.b, 1.0f);
	}

    /**
     * Clear the screen. Should be called per frame.
     * Tip: Use a color like pink. This will help development to find bugs.
     * This works great if we use a sky dome, which should always cover the
     * OpenGL background.
     */
	inline void clear(GLuint mode = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)
	{
		glClear(mode);
	}

	/**
	 * Set render modes, like solid or wireframe mode.
	 * @param mode Solid, wireframe or point render modes.
	 */
	inline void setPolygonMode(PolygonMode mode) {
		GLuint polyMode;
		switch (mode) {
		case PolygonMode::SOLID: polyMode = GL_FILL; break;
		case PolygonMode::WIREFRAME: polyMode = GL_LINE; break;
		case PolygonMode::POINT: polyMode = GL_POINT; break;
		default: polyMode = 0x00;
		}

		// Face should only be GL_FRONT_AND_BACK
		glPolygonMode(GL_FRONT_AND_BACK, polyMode);
	}

    /**
     * Draw indexed objects.
     * This only works if the object uses indices (Element Buffer Object).
     * @param vertexArrayObject Vertex Array Object to bind
     * @param primitive OpenGL primitive to draw with. Default: GL_TRIANGLES
     */
	inline void drawIndex(
		const Framework::VertexArray& vertexArrayObject,
	 	GLenum primitive = GL_TRIANGLES) {
		
		vertexArrayObject.bind();

		uint32_t count = vertexArrayObject.getIndexBuffer()->getCount();

		glDrawElements(
			GL_TRIANGLES,
            (int)count,
			GL_UNSIGNED_INT,
			nullptr
		);
	}

    inline void drawIndex(const std::shared_ptr<Framework::VertexArray>& vao,
                          GLenum primitive = GL_TRIANGLES) {
        drawIndex(*vao, primitive);
    }
}

#endif // RENDER_COMMANDS_H