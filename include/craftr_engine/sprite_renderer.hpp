#ifndef CRAFTR_SPRITE_RENDERER_HPP
#define CRAFTR_SPRITE_RENDERER_HPP

#include "craftr_engine/game_object.hpp"
#include "craftr_editor/property.hpp"
#include "craftr_editor/component_registry.hpp"
#include <string>

class SpriteRenderer : public Component {
public:
  // Color channels in [0,1]
  float red = 1.0f;
  float green = 1.0f;
  float blue = 1.0f;
  float alpha = 1.0f;

  SpriteRenderer();
  ~SpriteRenderer() override = default;

  std::string get_name() const override { return "SpriteRenderer"; }
};

REGISTER_COMPONENT(SpriteRenderer)

#endif 