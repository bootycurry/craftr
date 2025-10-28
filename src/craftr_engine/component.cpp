#include "craftr_engine/game_object.hpp"

GameObject* Component::get_game_object()
{
    return game_object;
}

const std::vector<Property>& Component::get_properties() const{ return properties; }

std::string Component::get_name() const {return typeid(*this).name();}

void Component::set_game_object(GameObject &object) {
    game_object = &object;
}
