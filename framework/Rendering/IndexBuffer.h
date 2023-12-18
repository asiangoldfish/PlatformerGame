#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

#include <glad/glad.h>
#include <memory>

namespace Framework {
    /**
     * The IndexBuffer class represents the Element Buffer Object in OpenGL.
     */
    class IndexBuffer {
    public:
        /**
         * Construct the IndexBuffer class.
         *
         * During construction, create a new OpenGL Element Buffer Object.
         * @param indices Indices to upload to the GPU.
         * @param count How many indices to upload.
         *
         * @example
         * std::vector indices; // Generate the indices here
         * IndexBuffer* indexBuffer =
         *      new IndexBuffer(indices.data(), indices.size());
         */
        IndexBuffer(GLuint *indices, GLsizei count);
        virtual ~IndexBuffer();

        /** Bind this index buffer */
        void bind() const;
        /** Unbind this index buffer */
        void unbind() const;

        /** Get the count of how many indices there are in this buffer */
        inline GLuint getCount() const { 
            return count;
        }

        /**
         * Shorthand for creating a new IndexBuffer shared pointer.
         *
         * @example
         * // Put the correct arguments for ::create()
         * auto indexBuffer = IndexBuffer::create();
         */
        static std::shared_ptr <IndexBuffer> create(
            GLuint* indices, GLsizei count);
    
    private:
        /** This index buffer's ID */
        GLuint indexBufferID;
        /** How many indices there are in the buffer */
        GLuint count;
    };
}

#endif // INDEX_BUFFER_H