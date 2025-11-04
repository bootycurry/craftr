#ifndef CRAFTR_GAMEOBJECT_HPP
#define CRAFTR_GAMEOBJECT_HPP

#include <memory>
#include <string>
#include <typeindex>
#include <unordered_map>
#include <vector>

#include "craftr_editor/property.hpp"
#include "logger.hpp"
#include "vector2.hpp"

class GameObject;

class Component {

protected:
  GameObject *game_object;
  void set_game_object(GameObject &object);
  std::vector<Property> properties;

public:
  GameObject *get_game_object();
  const std::vector<Property> &get_properties() const;
  virtual ~Component() = default;
  virtual std::string get_name() const;

  friend GameObject;
};

class Transform : public Component {
protected:
  Vector2 local_position;
  Vector2 local_scale;
  float local_rotation;
  bool is_updating_from_inspector; // Prevent recursive updates


public:
  Vector2 position;
  Vector2 scale;
  float rotation;

  void initialize_properties();

  Vector2 up() const;
  Vector2 right() const;

  Transform();

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

class GameObject {

protected:
  std::unordered_map<std::type_index, std::unique_ptr<Component>> components;
  std::vector<GameObject *> children;
  GameObject *parent = nullptr;

public:
  std::string name;
  Transform transform;

  template <typename T> void add_component();

  template <typename T> T *get_component();

  const std::unordered_map<std::type_index, std::unique_ptr<Component>> &
  get_all_components() const;

  void add_child(GameObject &child);

  void set_parent(GameObject &parent);

  GameObject *get_parent();
  const std::vector<GameObject *> get_children() const;
  GameObject();
  GameObject(GameObject *parent);
};

template <typename T> void GameObject::add_component() {
  static_assert(std::is_base_of<Component, T>::value,
                "T must derive from Component");
  static_assert(!std::is_same<T, Transform>::value,
                "Adding Transform component is not allowed");
  auto typeId = std::type_index(typeid(T));

  if (components.find(typeId) != components.end()) {
    throw std::runtime_error("Component already exists on the object");
  }

  T* component = new T();
  component->set_game_object(*this);
  components[typeId] = std::unique_ptr<Component>(component);
}

template <typename T> T* GameObject::get_component() {
  static_assert(std::is_base_of<Component, T>::value,
                "T must derive from Component");
  auto it = components.find(std::type_index(typeid(T)));
  if (it != components.end()) {
    return dynamic_cast<T*>(it->second.get());
  }
  return nullptr;
}

#endif
