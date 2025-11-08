#ifndef CRAFTR_C_API_GAMEOBJECT_H
#define CRAFTR_C_API_GAMEOBJECT_H

#include "../craftr_export.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* Craftr_GameObject;

// lifecycle
CRAFTR_API Craftr_GameObject craftr_create_game_object();
CRAFTR_API void craftr_destroy_game_object(Craftr_GameObject obj);

// name handling: returns newly-allocated string 
CRAFTR_API void craftr_set_gameobject_name(Craftr_GameObject obj, const char* name);
CRAFTR_API char* craftr_get_gameobject_name(Craftr_GameObject obj);

// hierarchy
CRAFTR_API void craftr_set_parent(Craftr_GameObject child, Craftr_GameObject parent);
CRAFTR_API Craftr_GameObject craftr_get_parent(Craftr_GameObject obj);
CRAFTR_API int craftr_get_child_count(Craftr_GameObject obj);
CRAFTR_API Craftr_GameObject craftr_get_child(Craftr_GameObject obj, int index);

// transform
CRAFTR_API void craftr_set_position(Craftr_GameObject obj, float x, float y);
CRAFTR_API void craftr_get_position(Craftr_GameObject obj, float* out_x, float* out_y);

CRAFTR_API void craftr_set_rotation(Craftr_GameObject obj, float rot);
CRAFTR_API float craftr_get_rotation(Craftr_GameObject obj);

CRAFTR_API void craftr_set_scale(Craftr_GameObject obj, float x, float y);
CRAFTR_API void craftr_get_scale(Craftr_GameObject obj, float* out_x, float* out_y);

// components (example)
CRAFTR_API void craftr_update_game_object(Craftr_GameObject obj);
CRAFTR_API void craftr_add_sprite_renderer(Craftr_GameObject obj);
CRAFTR_API int craftr_has_sprite_renderer(Craftr_GameObject obj);

#ifdef __cplusplus
}
#endif

#endif // CRAFTR_C_API_GAMEOBJECT_H