// External libraries
#include <glad/glad.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <stb_image.h>
#include <glm/gtc/matrix_transform.hpp>

// Framework
#include "Model.h"
#include "TextureManager.h"

namespace Framework {
#pragma region Mesh
    Mesh::Mesh(const std::vector<Vertex>& v,
               const std::vector<unsigned int>& i,
               const std::vector<SimpleTexture>& t)
      : vertices(v)
      , indices(i)
      , textures(t)
    {
        setupMesh();
    }

    void Mesh::draw(Shader& shader)
    {
        uint32_t diffuseNr = 1;
        uint32_t specularNr = 1;

        for (uint32_t i = 0; i < textures.size(); i++) {
            //            glActiveTexture(GL_TEXTURE0 + i);
            std::string number;
            std::string name = textures[i].type;
            //            if (name == "texture_diffuse") {
            ////                number = std::to_string(diffuseNr++);
            //                TE
            //            } else if (name == "texture_specular") {
            //                number = std::to_string(specularNr++);
            //                glActiveTexture(GL_TEXTURE1);
            //            } else {
            //                framework_assert("Mesh::draw(): Unsupported
            //                texture of type: " +
            //                                 name);
            //            }

            TextureManager::bind((uint32_t)textures[i].id, 0);
        }

        glActiveTexture(GL_TEXTURE0);

        // Draw mesh
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        glBindVertexArray(0);
    }

    /*
     * Allocate memory on the GPU to store the vertex buffer and index buffer.
     */
    void Mesh::setupMesh()
    {
        glGenVertexArrays(1, &vao);
        glGenBuffers(1, &vbo);
        glGenBuffers(1, &ebo);

        glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);

        // Load vertices to vertex buffer
        glBufferData(GL_ARRAY_BUFFER,
                     vertices.size() * sizeof(Vertex),
                     &vertices[0],
                     GL_STATIC_DRAW);

        // Load indices to element buffer
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     indices.size() * sizeof(uint32_t),
                     &indices[0],
                     GL_STATIC_DRAW);

        // Vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(
          0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

        // Vertex color attributes
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1,
                              4,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void*)offsetof(Vertex, color));

        // Vertex texture coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2,
                              2,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void*)offsetof(Vertex, texCoords));
        // Vertex normals
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3,
                              3,
                              GL_FLOAT,
                              GL_FALSE,
                              sizeof(Vertex),
                              (void*)offsetof(Vertex, normal));

        glBindVertexArray(0);
    }
#pragma endregion

#pragma region Model
    // Draw the model to screen
    void Model::draw(Shader& shader)
    {
        glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), this->scale);

        glm::mat4 transformationMatrix = translate * scale;

        shader.setMat4("u_model", transformationMatrix);

        for (auto& mesh : meshes) {
            mesh.draw(shader);
        }
    }

    // Helper function to load the model
    void Model::loadModel(std::string path)
    {
        Assimp::Importer importer;
        const aiScene* scene =
          importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE ||
            !scene->mRootNode) {
            WARN("ERROR:ASSIMP::{}", importer.GetErrorString());
            return;
        }

        directory = path.substr(0, path.find_last_of('/'));

        processNode(scene->mRootNode, scene);
    }

    // Process each of the scene tree's nodes. Store them in the meshes vector.
    void Model::processNode(aiNode* node, const aiScene* scene)
    {
        // Process all the node's meshes (if any)
        for (uint32_t i = 0; i < node->mNumMeshes; i++) {
            // Get the mesh from each node
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            // Process before putting it to meshes vector
            meshes.push_back(processMesh(mesh, scene));
        }

        // Do the same thing for each child
        for (uint32_t i = 0; i < node->mNumChildren; i++) {
            processNode(node->mChildren[i], scene);
        }
    }

    Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        std::vector<SimpleTexture> textures;

        for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
            Vertex vertex;

            // Process vertex positions
            vertex.position = { mesh->mVertices[i].x,
                                mesh->mVertices[i].y,
                                mesh->mVertices[i].z };

            // Process colors
            if (mesh->mColors[0]) {
                vertex.color = { mesh->mColors[0]->r,
                                 mesh->mColors[0]->g,
                                 mesh->mColors[0]->b,
                                 mesh->mColors[0]->a };
            } else {
                vertex.color = glm::vec4(1.0f);
            }

            // Process texture coordinates
            vertex.texCoords = { mesh->mTextureCoords[0]->x,
                                 mesh->mTextureCoords[0]->y };

            // Process vertex normals
            vertex.normal = { mesh->mNormals[i].x,
                              mesh->mNormals[i].y,
                              mesh->mNormals[i].z };

            vertices.push_back(vertex);
        };

        // Process indices
        for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
            aiFace face = mesh->mFaces[i];

            for (uint32_t j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        // Process material
        if (mesh->mMaterialIndex >= 0) {
            aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

            std::vector<SimpleTexture> diffuseMaps = loadMaterialTextures(
              material, aiTextureType_DIFFUSE, "texture_diffuse");
            textures.insert(
              textures.end(), diffuseMaps.begin(), diffuseMaps.end());

            std::vector<SimpleTexture> specularMaps = loadMaterialTextures(
              material, aiTextureType_SPECULAR, "texture_specular");
            textures.insert(
              textures.end(), specularMaps.begin(), specularMaps.end());
        }

        return { vertices, indices, textures };
    }

    std::vector<SimpleTexture> Model::loadMaterialTextures(aiMaterial* mat,
                                                           aiTextureType type,
                                                           std::string typeName)
    {
        std::vector<SimpleTexture> textures;

        for (uint32_t i = 0; i < mat->GetTextureCount(type); i++) {
            aiString str;
            mat->GetTexture(type, i, &str);

            SimpleTexture texture;

            // We only load texture from disk if it doesn't already exist
            Texture* foundTexture = nullptr;
            for (const auto& t : TextureManager::getTextures()) {
                if (directory + '/' + std::string(str.C_Str()) ==
                    t->getFilepath()) {
                    foundTexture = t.get();
                }
            }

            // Set texture ID. Load from disk if it isn't already loaded in
            // TextureManager.
            if (!foundTexture) {
                int slot;
                if (type == aiTextureType_DIFFUSE) {
                    slot = 0;
                } else if (type == aiTextureType_SPECULAR) {
                    slot = 1;
                }
                // TODO: Fix the below
//                texture.id = TextureManager::loadTexture(
//                    shader,
//                  str.C_Str(),
//                  directory + '/' + std::string(str.C_Str()),
//                  TextureManager::Texture2D,
//                  slot);
            } else {
                texture.id = foundTexture->getTextureId();
            }
            texture.type = typeName;
            textures.push_back(texture);
        }

        return textures;
    }

#pragma endregion
} // Framework
