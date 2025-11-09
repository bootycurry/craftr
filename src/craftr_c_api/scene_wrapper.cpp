
#include "craftr_c_api/scene.h"
#include "craftr_engine/game_object.hpp"
#include "craftr_engine/scene_graph.hpp"

extern "C" {

CRAFTR_API void craftr_register_game_object(Craftr_GameObject obj) {
  if (!obj)
    return;
  GameObject *go = reinterpret_cast<GameObject *>(obj);
  SceneGraph::instance().register_game_object(go);
}
}
