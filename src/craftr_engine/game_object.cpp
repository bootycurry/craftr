#include <stdexcept>

#include "craftr_engine/game_object.hpp"

template <typename T> void GameObject::add_component() {
  static_assert(std::is_base_of<Component, T>::value,
                "T must derive from Component");
  static_assert(!std::is_same<T, Transform>::value,
                "Adding Transform component is not allowed");
  auto typeId = std::type_index(typeid(T));

  if (components.find(typeId) != components.end()) {
    throw std::runtime_error("Component already exists on the object");
  }

  T *component = new T();
  component->set_game_object(this);
  components[typeId] = std::unique_ptr<Component>(component);
}

template <typename T> T *GameObject::get_component() {
  static_assert(std::is_base_of<Component, T>::value,
                "T must derive from Component");
  auto it = components.find(std::type_index(typeid(T)));
  if (it != components.end()) {
    return dynamic_cast<T *>(it->second.get());
  }
  return nullptr;
}

void GameObject::set_parent(GameObject &parent) {
  this->parent = &parent;
  parent.children.push_back(this);
}

void GameObject::add_child(GameObject &child) {
  children.push_back(&child);
  child.parent = this;
}

GameObject *GameObject::get_parent() { return parent; }

std::vector<GameObject *> GameObject::get_children() { return children; }
