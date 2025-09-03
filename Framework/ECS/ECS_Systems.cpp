#include "ECS_Systems.h"

#include <algorithm>

namespace FW {
    void fetchEntities(ref<SceneNode> sceneRoot,
                       std::vector<Entity*>& entities) {
        if (sceneRoot->entity) {
            auto drawableComponent =
              sceneRoot->entity->getComponent<TransformationComponent>();
            if (drawableComponent) {
                entities.push_back(sceneRoot->entity.get());
            }
        }

        for (auto& node : sceneRoot->childNodes) {
            fetchEntities(node, entities);
        }
    }

    RenderSystem::RenderSystem() {}

    void RenderSystem::draw(ref<SceneNode> sceneRoot) {
        // The draw algorithm separates transparent and opaque entities. This
        // means we can enable blend onlny for the transparent objects.
        
        std::vector<Entity*> drawableEntities;
        std::size_t reserveNumber = 20;
        // Change the number to increase how many entities to increase by.
        drawableEntities.reserve(reserveNumber);

        fetchEntities(sceneRoot, drawableEntities);

        std::sort(drawableEntities.begin(),
                  drawableEntities.end(),
                  [](Entity* a, Entity* b) {
                      return a->getComponent<DrawableComponent>()->Z_index <
                             b->getComponent<DrawableComponent>()->Z_index;
                  });

        std::vector<Entity*> opaqueEntities;
        opaqueEntities.reserve(reserveNumber);
        std::vector<Entity*> transparentEntities;
        transparentEntities.reserve(reserveNumber);

        for (const auto& entity : drawableEntities) {
            if (entity->getComponent<DrawableComponent>()->isTransparent) {
                transparentEntities.push_back(entity);
            } else {
                opaqueEntities.push_back(entity);
            }
        }

        for (const auto& entity : opaqueEntities) {
            entity->getComponent<TransformationComponent>()
              ->uploadTransformationMatrix();
            entity->getComponent<DrawableComponent>()->draw();
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);
        for (const auto& entity : transparentEntities) {
            entity->getComponent<TransformationComponent>()
              ->uploadTransformationMatrix();
            entity->getComponent<DrawableComponent>()->draw();
        }
    }
}