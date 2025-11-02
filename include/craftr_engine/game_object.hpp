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

public:
  Vector2 position;
  Vector2 scale;
  float rotation;

  Vector2 up();
  Vector2 right();

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
  GameObject *parent;

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
  std::vector<GameObject *> get_children();
  GameObject();
  GameObject(GameObject *parent);
};

#endif
