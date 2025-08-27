#include <cmath>
#include <numbers>

#include "craftr_engine\game_object.hpp"

Vector2 Transform::right()
{
    double angle = (rotation/180) * std::numbers::pi;
    return(Vector2(cos(angle),sin(angle)));
}

Vector2 Transform::right()
{
    double angle = ((rotation + 90)/180) * std::numbers::pi;
    return(Vector2(cos(angle),sin(angle)));
}

void Transform::set_local_position(Vector2 pos)
{
    if(game_object -> get_parent() == nullptr)
    {
        set_position(pos);
    }

    else
    {
        Transform parent_transform = game_object -> get_parent() -> transform;
        Vector2 up = parent_transform.up();
        Vector2 right = parent_transform.right();
        set_position(parent_transform.position + pos.x * right + pos.y * up);
    }
    local_position = pos;
}

void Transform::set_local_rotation(float rot)
{
    if(game_object -> get_parent() == nullptr)
    {
        set_rotation(rot);
    }

    else
    {
        set_rotation(game_object -> get_parent() -> transform.rotation + rot);
    }
    local_rotation = rot;
}

void Transform::set_local_scale(Vector2 scale)
{
    if(game_object -> get_parent() == nullptr)
    {
        set_scale(scale);
    }

    else
    {
        Vector2 parent_scale = game_object -> get_parent() -> transform.scale;
        set_scale(Vector2(parent_scale.x * scale.x, parent_scale.y * scale.y));
    }
    local_scale = scale;
}

void Transform::set_position(Vector2 pos)
{
    position = pos;
    if(GameObject* parent = game_object -> get_parent())
    {
        Vector2 d = position - parent -> transform.position;
        Vector2 up = parent -> transform.up();
        Vector2 right = parent -> transform.right();
        local_position = Vector2(d.x * up.x - d.y * up.x, d.y*right.x - d.x * right.y);
    }
    else
    {
        local_position = position;
    }
    for (auto i : game_object -> get_children())
    {
        i -> transform.update_position();
    }
}

void Transform::set_rotation(float rot)
{

    rotation = rot;
    if(GameObject* parent = game_object -> get_parent())
    {
        local_rotation = parent -> transform.rotation - rotation;
    }
    else
    {
        local_rotation = rotation;
    }
    for (auto i : game_object -> get_children())
    {
        i -> transform.update_rotation();
        i -> transform.update_position();
    }
}

void Transform::set_scale(Vector2 scale)
{
    this -> scale = scale;
    if(GameObject* parent = game_object -> get_parent())
    {
        local_scale = Vector2(parent -> transform.scale.x / scale.x,parent -> transform.scale.y / scale.y);
    }
    else
    {
        local_scale = scale;
    }
    for (auto i : game_object -> get_children())
    {
        i -> transform.update_scale();
    }
}

void Transform::update_position()
{
    set_local_position(local_position);
}

void Transform::update_rotation()
{
    set_local_rotation(local_rotation);
}

void Transform::update_scale()
{
    set_local_scale(local_scale);
}