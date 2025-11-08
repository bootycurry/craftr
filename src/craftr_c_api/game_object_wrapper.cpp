#include "craftr_c_api/game_object.h"
#include "craftr_engine/game_object.hpp"
#include "craftr_engine/sprite_renderer.hpp"
#include <cstring>
#include <cstdlib> 
#include <new>

extern "C" {

CRAFTR_API Craftr_GameObject craftr_create_game_object() {
    try {
        GameObject* go = new GameObject();
        std::printf("Created GameObject at %p\n", go);
        std::fflush(stdout);
        return reinterpret_cast<Craftr_GameObject>(go);
    } catch (const std::exception& e) {
        std::fprintf(stderr, "Exception: %s\n", e.what());
        return nullptr;
    } catch (...) {
        std::fprintf(stderr, "Unknown exception!\n");
        return nullptr;
    }
}

CRAFTR_API void craftr_destroy_game_object(Craftr_GameObject obj) {
    if (!obj) return;
    delete reinterpret_cast<GameObject*>(obj);
}

CRAFTR_API void craftr_set_gameobject_name(Craftr_GameObject obj, const char* name) {
    if (!obj || !name) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    go->name = std::string(name);
}

CRAFTR_API char* craftr_get_gameobject_name(Craftr_GameObject obj) {
    if (!obj) return nullptr;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    const std::string &s = go->name;
    // allocate a C-string that the caller must free
    size_t len = s.size();
    char* out = (char*)std::malloc(len + 1);
    if (!out) return nullptr;
    std::memcpy(out, s.c_str(), len + 1);
    return out;
}

CRAFTR_API void craftr_set_parent(Craftr_GameObject child, Craftr_GameObject parent) {
    if (!child || !parent) return;
    GameObject* c = reinterpret_cast<GameObject*>(child);
    GameObject* p = reinterpret_cast<GameObject*>(parent);
    c->set_parent(*p);
}

CRAFTR_API Craftr_GameObject craftr_get_parent(Craftr_GameObject obj) {
    if (!obj) return nullptr;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    return reinterpret_cast<Craftr_GameObject>(go->get_parent());
}

CRAFTR_API int craftr_get_child_count(Craftr_GameObject obj) {
    if (!obj) return 0;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    return static_cast<int>(go->get_children().size());
}

CRAFTR_API Craftr_GameObject craftr_get_child(Craftr_GameObject obj, int index) {
    if (!obj) return nullptr;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    const auto children = go->get_children();
    if (index < 0 || index >= static_cast<int>(children.size())) return nullptr;
    return reinterpret_cast<Craftr_GameObject>(children[index]);
}

CRAFTR_API void craftr_set_position(Craftr_GameObject obj, float x, float y) {
    if (!obj) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    go->transform.set_position(Vector2(x, y));
}

CRAFTR_API void craftr_get_position(Craftr_GameObject obj, float* out_x, float* out_y) {
    if (!obj || !out_x || !out_y) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    *out_x = go->transform.position.x;
    *out_y = go->transform.position.y;
}

CRAFTR_API void craftr_set_rotation(Craftr_GameObject obj, float rot) {
    if (!obj) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    go->transform.set_rotation(rot);
}

CRAFTR_API float craftr_get_rotation(Craftr_GameObject obj) {
    if (!obj) return 0.0f;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    return go->transform.rotation;
}

CRAFTR_API void craftr_set_scale(Craftr_GameObject obj, float x, float y) {
    if (!obj) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    go->transform.set_scale(Vector2(x, y));
}

CRAFTR_API void craftr_get_scale(Craftr_GameObject obj, float* out_x, float* out_y) {
    if (!obj || !out_x || !out_y) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    *out_x = go->transform.scale.x;
    *out_y = go->transform.scale.y;
}

CRAFTR_API void craftr_add_sprite_renderer(Craftr_GameObject obj) {
    if (!obj) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    try {
        go->add_component<SpriteRenderer>();
    } catch (...) {
        // ignore if already exists
    }
}

CRAFTR_API int craftr_has_sprite_renderer(Craftr_GameObject obj) {
    if (!obj) return 0;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    return go->get_component<SpriteRenderer>() ? 1 : 0;
}

} 

CRAFTR_API void craftr_update_game_object(Craftr_GameObject obj) {
    if (!obj) return;
    GameObject* go = reinterpret_cast<GameObject*>(obj);
    go->update();
}