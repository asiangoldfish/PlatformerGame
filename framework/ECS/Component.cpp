#include "Component.h"
#include "TextureManager.h"

#include "glm/gtc/matrix_transform.hpp"

void FW::DrawableComponent::init() {
}

void FW::DrawableComponent::update(float delta) {
    draw();
}

void FW::DrawableComponent::draw() {
    shader->bind();
    shader->setFloat4("u_color", glm::vec4(1.0, 1.0, 1.0, 1.0));

    // Upload material properties
    //        shader->setFloat3("u_material.ambient",
    //        material.getProperties().ambient);
    //        shader->setFloat3("u_material.diffuse",
    //        material.getProperties().diffuse);

    // We allow the user to either upload a texture or set it by value
    // -----
    // Diffuse
    FW::TextureManager::bind(material.getProperties().diffuseTextureID,
                                0);

    // Specular
    /*
    if (material.getProperties().specularTextureId != -1) {
        FW::TextureManager::bind(material.getProperties().specularTextureId);
    } else if (material.getProperties().isSpecularTextureSet()) {
        FW::TextureManager::bind(
            material.getProperties().specularTextureName);
    } else {
        FW::TextureManager::bind("no-texture-spec");
    }
        */

    // Shininess
    shader->setFloat("u_material.shininess",
                        material.getProperties().shininess);

    RenderCommand::drawIndex(shape->getVertexArray());
}

void FW::TransformationComponent::init() {}

void FW::TransformationComponent::update(float delta) {
    // TODO find some way to avoid recalculating the model matrix every frame
    recalculateModelMatrix();
    shader->bind();
    shader->setMat4("u_model", modelMatrix);
}

void FW::TransformationComponent::setPosition(glm::vec3 position) {
    this->position = position;
    recalculateModelMatrix();
}

void FW::TransformationComponent::setScale(float x, float y, float z) {
    scale.x = x;
    scale.y = y;
    scale.z = z;
    recalculateModelMatrix();
}

void FW::TransformationComponent::recalculateModelMatrix() {
    modelMatrix = glm::mat4(1.0f);
        // Translation

        glm::mat4 newRotation, rotx, roty, rotz;

        // Rotation
        rotx =
          glm::rotate(modelMatrix, glm::radians(yaw), { 1.0f, 0.0f, 0.0f });
        roty =
          glm::rotate(modelMatrix, glm::radians(pitch), { 0.0f, 1.0f, 0.0f });
        rotz =
          glm::rotate(modelMatrix, glm::radians(roll), { 0.0f, 0.0f, 1.0f });

        newRotation = rotx * roty * rotz;

        modelMatrix = glm::translate(modelMatrix, position) * newRotation *
                      glm::scale(modelMatrix, scale);
}
