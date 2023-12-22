#pragma once

#include "pch.h"

// External libraries
#include <assimp/scene.h>
#include <glm/glm.hpp>

// Framework
#include "Shader.h"
#include "Shader.h"

namespace Framework {
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoords;
        glm::vec3 normal;
    };

    struct SimpleTexture
    {
        uint32_t id;
        std::string type;
    };

    class Mesh
    {
    public:
        Mesh(const std::vector<Vertex>& v,
             const std::vector<unsigned int>& i,
             const std::vector<SimpleTexture>& t);

        void draw(Shader& shader);

        [[nodiscard]] const std::vector<uint32_t>& getIndices() const
        {
            return indices;
        }
        void setIndices(const std::vector<uint32_t>& i) { indices = i; }

        [[nodiscard]] const std::vector<Vertex>& getVertices() const
        {
            return vertices;
        }
        void setVertices(const std::vector<Vertex>& v) { vertices = v; }

        [[nodiscard]] const std::vector<SimpleTexture>& getTextures() const
        {
            return textures;
        }
        void setTextures(const std::vector<SimpleTexture>& t) { textures = t; }

    private:
        void setupMesh();

    private:
        uint32_t vao, vbo, ebo;

        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<SimpleTexture> textures;
    };

    class Model
    {
    public:
        /** Construct an empty model. To load a model, use the copy constructor
         * to load a model */
        Model() = default;

        /**
         * Construct a model by loading it from disk
         *
         * @param path The file path on disk
         */
        Model(char* path) { loadModel(path); }

        /**
         * Construct a model by loading it from disk
         *
         * @param path The file path on disk
         */
        Model(const std::string& path) { loadModel(path); }

        /**
         * Draw the model
         * @param shader The shader that model data is uploaded to.
         */
        void draw(Shader& shader);

        void setPosition(const glm::vec3& pos) { position = pos; }
        void setScale(const glm::vec3& value) { scale = value; }
        void setScale(const float value)
        {
            scale = glm::vec3(value, value, value);
        }

    private:
        /**
         * Helper function to load the model from disk.
         * @param path Where on the disk the model is at.
         */
        void loadModel(std::string path);

        /**
         * Process each node of the Assimp scene tree.
         *
         * @param node The node to process
         * @param scene The scene it belongs to
         */
        void processNode(aiNode* node, const aiScene* scene);

        /**
         * Process the mesh and each of its parts
         *
         * @param mesh The mesh to support
         * @param scene The scene that the mesh resides
         * @return Processed mesh from Assimp's mesh.
         */
        Mesh processMesh(aiMesh* mesh, const aiScene* scene);

        std::vector<SimpleTexture> loadMaterialTextures(aiMaterial* mat,
                                                        aiTextureType type,
                                                        std::string typeName);

    private:
        // Model data
        std::vector<Mesh> meshes;
        std::string directory;

        glm::vec3 position{0.0f};
        glm::vec3 scale{1.0f};
    };
} // Framework
