#pragma once

#include "Box.h"
#include "Memory.h"
#include "Camera/Camera.h"

namespace FW::UI {
    class ProgressBar : public Sprite {
    public:
        ProgressBar();
        ProgressBar(ref<Camera> camera);
        virtual ~ProgressBar() = default;

        void init();

    private:
        FW::ref<Box> background;
        FW::ref<Box> foreground;
    };
}