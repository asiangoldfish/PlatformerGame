#pragma once

#include "Memory.h"
#include "Camera/Camera.h"
#include "UIRoot.h"
#include "Sprite.h"

namespace FW::UI {
    class ProgressBar : public UIRoot {
    public:
        ProgressBar() = default;
        ProgressBar(ref<Camera> camera);
        virtual ~ProgressBar() = default;

        void init();

    public:
        FW::ref<FW::Sprite> background;
        FW::ref<FW::Sprite> foreground;
    };
}