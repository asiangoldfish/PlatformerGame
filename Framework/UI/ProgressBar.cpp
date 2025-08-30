#include "ProgressBar.h"

namespace FW::UI {
    ProgressBar::ProgressBar() {
        init();
    }
    ProgressBar::ProgressBar(ref<Camera> camera)
      : Sprite(camera) {
        init();
    }

    void ProgressBar::init() {
        isDrawable = false;

        background = FW::createRef<Box>(camera);
        foreground = FW::createRef<Box>(camera);

        addChild(background);
        addChild(foreground);

        setSize(0.3f);
    }
}
