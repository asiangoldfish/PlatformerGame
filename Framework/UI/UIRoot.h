#pragma once

#include "Entity.h"
#include "Camera/Camera.h"

namespace FW::UI {
    /**
     * This class is a barebone class for UI with minimal functionality.
     *
     * By default, it is composed by the transformation component only, and its
     * primary purpose is to group child nodes.
     */
    class UIRoot : public Entity {
    public:
        UIRoot() = default;
        virtual ~UIRoot() = default;

        void init();
        ref<TransformationComponent> getTransform() {
            return transformationComponent;
        }

    private:
        ref<TransformationComponent> transformationComponent;
        std::string shader;
    };
}