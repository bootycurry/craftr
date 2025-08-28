#ifndef CRAFTR_GAMEOBJECT_HPP
#define CRAFTR_GAMEOBJECT_HPP

#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include <vector>

#include "logger.hpp"
#include "vector2.hpp"

class GameObject;

class Component{
    
    protected:
        GameObject* game_object;
        void set_game_object(GameObject& object);

    public:
        GameObject* get_game_object();


    friend GameObject;
};

class Transform : Component{
    protected:
        Vector2 local_position;
        Vector2 local_scale;
        float local_rotation;

    public:
        Vector2 position;
        Vector2 scale;
        float rotation;

        Vector2 up();
        Vector2 right();

        void set_local_position(Vector2 pos);
        void set_local_rotation(float rot);
        void set_local_scale(Vector2 scale);

        void set_position(Vector2 pos);
        void set_rotation(float rot);
        void set_scale(Vector2 scale);

        void update_position();
        void update_rotation();
        void update_scale();
    
};

class GameObject{

    protected:
        std::unordered_map<std::type_index,std::unique_ptr<Component>>  components;
        std::vector<GameObject*> children;
        GameObject* parent;

    public:
        std::string name;
        Transform transform;

        template <typename T>
        void add_component();

        template <typename T>
        T* get_component();

        void add_child(GameObject& child);

        void set_parent(GameObject& parent);

        GameObject* get_parent();
        std::vector<GameObject*> get_children();

};


#endif