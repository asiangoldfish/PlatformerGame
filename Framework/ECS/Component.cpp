#include "Component.h"
#include "TextureManager.h"
#include "ShaderManager.h"

#include "glm/gtc/matrix_transform.hpp"

void FW::DrawableComponent::init() {}

void FW::DrawableComponent::update(float delta) {
    draw();
}

void FW::DrawableComponent::draw() {
    auto shaderRef = ShaderManager::get().bind(shader);
    if (!shaderRef) {
        return;
    }

    shaderRef->setFloat4("u_color", color);

    // Upload material properties
    //        shader->setFloat3("u_material.ambient",
    //        material.getProperties().ambient);
    //        shader->setFloat3("u_material.diffuse",
    //        material.getProperties().diffuse);

    // We allow the user to either upload a texture or set it by value
    // -----
    // Diffuse
    FW::TextureManager::bind(material.getProperties().diffuseTextureID, 0);

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
    shaderRef->setFloat("u_material.shininess",
                     material.getProperties().shininess);

    RenderCommand::drawIndex(shape->getVertexArray());
}

void FW::TransformationComponent::init() {}

void FW::TransformationComponent::update(float delta) {
    // TODO find some way to avoid recalculating the model matrix every frame
    recalculateModelMatrix();
    auto shaderRef = ShaderManager::get().bind(shader);
    if (!shaderRef) {
        return;
    }
    
    shaderRef->setMat4("u_model", modelMatrix);
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
    rotx = glm::rotate(modelMatrix, yaw, { 1.0f, 0.0f, 0.0f });
    roty = glm::rotate(modelMatrix, pitch, { 0.0f, 1.0f, 0.0f });
    rotz = glm::rotate(modelMatrix, roll, { 0.0f, 0.0f, 1.0f });

    newRotation = rotx * roty * rotz;

    modelMatrix = glm::translate(modelMatrix, position) * newRotation *
                  glm::scale(modelMatrix, scale);
}

void FW::PhysicsComponent::update(float delta) {
    // Gravity
    // velocity.y -= gravity;
}

void FW::PhysicsComponent::setVelocity(float x, float y, float z) {
    velocity.x = x;
    velocity.y = y;
    velocity.z = z;
}

void FW::PhysicsComponent::addVelocity(float x, float y, float z) {
    velocity.x += x;
    velocity.y += y;
    velocity.z += z;
}

void FW::PhysicsComponent::addVelocity(float x, float y) {
    velocity.x += x;
    velocity.y += y;
}