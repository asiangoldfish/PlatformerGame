#include "Sprite.h"

Sprite::Sprite() {
    addComponent(transformationComponent);
    addComponent(drawableComponent);

    // A sprite consists of a square and an image. Therefore we should add them
    // via the drawable component.
    // drawableComponent->
}