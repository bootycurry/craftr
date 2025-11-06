#ifndef CRAFTR_C_API_GAMEOBJECT_H
#define CRAFTR_C_API_GAMEOBJECT_H

#ifdef __cplusplus
extern "C" {
#endif

typedef void* Craftr_GameObject;


Craftr_GameObject craftr_create_game_object();
void craftr_destroy_game_object(Craftr_GameObject obj);

void craftr_set_gameobject_name(Craftr_GameObject obj, const char* name);
const char* craftr_get_gameobject_name(Craftr_GameObject obj);

void craftr_set_parent(Craftr_GameObject child, Craftr_GameObject parent);
Craftr_GameObject craftr_get_parent(Craftr_GameObject obj);
int craftr_get_child_count(Craftr_GameObject obj);
Craftr_GameObject craftr_get_child(Craftr_GameObject obj, int index);


void craftr_set_position(Craftr_GameObject obj, float x, float y);
void craftr_get_position(Craftr_GameObject obj, float* out_x, float* out_y);

void craftr_set_rotation(Craftr_GameObject obj, float rot);
float craftr_get_rotation(Craftr_GameObject obj);

void craftr_set_scale(Craftr_GameObject obj, float x, float y);
void craftr_get_scale(Craftr_GameObject obj, float* out_x, float* out_y);


void craftr_add_sprite_renderer(Craftr_GameObject obj);
int craftr_has_sprite_renderer(Craftr_GameObject obj);

#ifdef __cplusplus
}
#endif

#endif // CRAFTR_C_API_GAMEOBJECT_H
