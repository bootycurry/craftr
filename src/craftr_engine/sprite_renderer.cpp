#include "craftr_engine/sprite_renderer.hpp"

// Register editable properties using REGISTER_PROPERTY macro
SpriteRenderer::SpriteRenderer() {
    REGISTER_PROPERTY(red);
    REGISTER_PROPERTY(green);
    REGISTER_PROPERTY(blue);
    REGISTER_PROPERTY(alpha);
}
