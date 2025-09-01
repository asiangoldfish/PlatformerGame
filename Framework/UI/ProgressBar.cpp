#include "ProgressBar.h"

namespace FW::UI {
    ProgressBar::ProgressBar(ref<Camera> camera){
        this->camera = camera;
        init();
    }

    void ProgressBar::init() {
        if (!camera) {{
            WARN("ProgressBar: Camera is uninitialized");
            return;
        }}

        background = FW::createRef<FW::Sprite>(camera);
        foreground = FW::createRef<FW::Sprite>(camera);

        addChild(background);
        addChild(foreground);
    }
}
