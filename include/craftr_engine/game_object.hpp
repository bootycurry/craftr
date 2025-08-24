#ifndef CRAFTR_GAMEOBJECT_HPP
#define CRAFTR_GAMEOBJECT_HPP

#include <string>
#include <unordered_map>
#include <typeindex>
#include <memory>

#include "logger.hpp"
#include "vector2.hpp"

class GameObject;

class Component{
    
    private:
        GameObject* game_object;
        void set_game_object(GameObject& object);

    public:
        GameObject* get_game_object();

        Component();
        ~Component();


    friend GameObject;
};

class Transform : Component{
    public:
        Vector2 position;
        Vector2 scale;
        float rotation;

        Transform();
        ~Transform();
    
};

class GameObject{

    protected:
        std::unordered_map<std::type_index,std::unique_ptr<Component>>  components;

    public:
        std::string name;
        Transform transform;

        template <typename T>
        void add_component();

        template <typename T>
        T* get_component();

        GameObject();
        ~GameObject();
};


#endif