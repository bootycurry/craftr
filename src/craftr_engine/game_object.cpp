#include <stdexcept>

#include "craftr_engine/game_object.hpp"

GameObject::GameObject(GameObject *parent) : parent(parent) {
  transform.set_game_object(*this);
  transform.initialize_properties();
}
GameObject::GameObject() {
  transform.set_game_object(*this);
  transform.initialize_properties();
};


void GameObject::set_parent(GameObject &parent) {
  this->parent = &parent;
  parent.children.push_back(this);
}

void GameObject::add_child(GameObject &child) {
  children.push_back(&child);
  child.parent = this;
}

GameObject *GameObject::get_parent() { return parent; }

const std::vector<GameObject *> GameObject::get_children() const { return children; }

const std::unordered_map<std::type_index, std::unique_ptr<Component>>&
GameObject::get_all_components() const {
  return components;
}

