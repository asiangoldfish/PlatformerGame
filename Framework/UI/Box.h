#include "Sprite.h"
#include "Component.h"

namespace FW::UI {
    /**
     * The Box is a wrapper, mainly for the Entity class and exposes common UI
     * functionalities.
     */
    class Box : public Sprite {
    public:
        Box();
        Box(ref<Camera> camera);
        virtual ~Box() = default;

    private:
    };
}