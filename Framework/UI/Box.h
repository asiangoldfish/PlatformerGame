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
            virtual ~Box() = default;

        private:
            glm::vec2 position;
            glm::vec2 size{100.0f};
            glm::vec3 color{1.0f};
    };
}