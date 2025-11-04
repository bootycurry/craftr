#include <cmath>
#include <numbers>

#include "craftr_engine/game_object.hpp"
#include "craftr_engine/vector2.hpp"
#include "logger.hpp"

Transform::Transform()
    : local_position(0, 0), local_scale(1, 1), local_rotation(0.0f),
      is_updating_from_inspector(false) {}


void Transform::initialize_properties() {
  REGISTER_PROPERTY_WITH_CALLBACK(local_position.x, [this]() {

    if (is_updating_from_inspector || !game_object) return;
    is_updating_from_inspector = true;
    set_local_position(Vector2(local_position.x, local_position.y));
    is_updating_from_inspector = false;
  });

  REGISTER_PROPERTY_WITH_CALLBACK(local_position.y, [this]() {
    if (is_updating_from_inspector || !game_object) return;
    is_updating_from_inspector = true;
    set_local_position(Vector2(local_position.x, local_position.y));
    is_updating_from_inspector = false;
  });

  REGISTER_PROPERTY_WITH_CALLBACK(local_rotation, [this]() {
    if (is_updating_from_inspector || !game_object) return;
    is_updating_from_inspector = true;
    set_local_rotation(local_rotation);
    is_updating_from_inspector = false;
  });

  REGISTER_PROPERTY_WITH_CALLBACK(local_scale.x, [this]() {
    if (is_updating_from_inspector || !game_object) return;
    is_updating_from_inspector = true;
    set_local_scale(Vector2(local_scale.x, local_scale.y));
    is_updating_from_inspector = false;
  });

  REGISTER_PROPERTY_WITH_CALLBACK(local_scale.y, [this]() {
    if (is_updating_from_inspector || !game_object) return;
    is_updating_from_inspector = true;
    set_local_scale(Vector2(local_scale.x, local_scale.y));
    is_updating_from_inspector = false;
  });

}


Vector2 Transform::right() const {
  double angle = (rotation / 180.0) * std::numbers::pi;
  return Vector2(cos(angle), sin(angle));
}

Vector2 Transform::up() const {
  double angle = ((rotation - 90.0) / 180.0) * std::numbers::pi;
  return Vector2(cos(angle), sin(angle));
}


void Transform::set_local_position(Vector2 pos) {
  if (!game_object) {
    local_position = pos;
    return;
  }

  if (game_object->get_parent() == nullptr) {
    set_position(pos);
  } else {
    const Transform &parent_transform = game_object->get_parent()->transform;
    Vector2 up = parent_transform.up();
    Vector2 right = parent_transform.right();
    set_position(parent_transform.position + pos.x * right + pos.y * up);
  }
}

void Transform::set_local_rotation(float rot) {
  if (!game_object) {
    local_rotation = rot;
    return;
  }

  if (game_object->get_parent() == nullptr) {
    set_rotation(rot);
  } else {
    set_rotation(game_object->get_parent()->transform.rotation + rot);
  }
}

void Transform::set_local_scale(Vector2 scale) {
  if (!game_object) {
    local_scale = scale;
    return;
  }

  if (auto *parent = game_object->get_parent()) {
    Vector2 parent_scale = parent->transform.scale;
    set_scale(Vector2(parent_scale.x * scale.x, parent_scale.y * scale.y));
  } else {
    set_scale(scale);
  }
}

void Transform::set_position(Vector2 pos) {
  if (!game_object) {
    position = pos;
    return;
  }

  position = pos;
  if (GameObject *parent = game_object->get_parent()) {
    Vector2 d = position - parent->transform.position;
    Vector2 up = parent->transform.up();
    Vector2 right = parent->transform.right();
    local_position =
        Vector2(d.x * right.x + d.y * right.y, d.x * up.x + d.y * up.y);
  } else {
    local_position = position;
  }

  for (auto *child : game_object->get_children()) {
    child->transform.update_position();
  }
}

void Transform::set_rotation(float rot) {
  if (!game_object) {
    rotation = rot;
    return;
  }

  rotation = rot;
  if (GameObject *parent = game_object->get_parent()) {
    local_rotation = rotation - parent->transform.rotation;
  } else {
    local_rotation = rotation;
  }

  for (auto *child : game_object->get_children()) {
    child->transform.update_rotation();
    child->transform.update_position();
  }
}




void Transform::set_scale(Vector2 scale) {
  if (!game_object) {
    this->scale = scale;
    return;
  }

  this->scale = scale;

  if (auto *parent = game_object->get_parent()) {
    Vector2 parent_scale = parent->transform.scale;
    local_scale = Vector2(
        parent_scale.x == 0 ? 0 : scale.x / parent_scale.x,
        parent_scale.y == 0 ? 0 : scale.y / parent_scale.y);
  } else {
    local_scale = scale;
  }

  for (auto *child : game_object->get_children()) {
    child->transform.update_scale();
  }
}


void Transform::update_position() { set_local_position(local_position); }
void Transform::update_rotation() { set_local_rotation(local_rotation); }
void Transform::update_scale() { set_local_scale(local_scale); }

void Transform::adjust_to_parent() {
  set_position(position);
  set_rotation(local_rotation);
  set_scale(local_scale);
}
