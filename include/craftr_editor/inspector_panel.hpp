#ifndef CRAFTR_INSPECTOR_PANEL_HPP
#define CRAFTR_INSPECTOR_PANEL_HPP

#include "craftr_editor/component_registry.hpp"
#include "craftr_engine/game_object.hpp"
#include <gtkmm.h>

class InspectorPanel : public Gtk::Box {
public:
  InspectorPanel();
  ~InspectorPanel() = default;

  void set_selected_object(GameObject *object);
  void refresh();

private:
  GameObject *selected_object = nullptr;

  Gtk::ScrolledWindow scrolled_window;
  Gtk::Box properties_box; // contains all component UI blocks
  Gtk::Button add_component_button;
  Gtk::PopoverMenu add_component_menu;

  void build_component_ui(Component *component);
  void show_add_component_menu();

  void clear();
};

#endif
