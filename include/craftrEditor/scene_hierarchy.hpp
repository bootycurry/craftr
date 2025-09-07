#ifndef CRAFTR_SCENE_HIERARCHY
#define CRAFTR_SCENE_HIERARCHY

#include <gdkmm.h>
#include <glibmm.h>
#include <gtkmm.h>
#include <memory>
#include <string>

#include "craftr_engine/game_object.hpp"
#include "glibmm/refptr.h"
#include "gtkmm/gestureclick.h"
#include "gtkmm/popovermenu.h"
#include "gtkmm/treemodel.h"

class SceneHierarchy : public Gtk::Box {
public:
  SceneHierarchy();
  ~SceneHierarchy() = default;

protected:
  class ModelColumns : public Gtk::TreeModel::ColumnRecord {
  public:
    ModelColumns() {
      add(col_name);
      add(game_obj_ptr);
    }

    Gtk::TreeModelColumn<Glib::ustring> col_name;
    Gtk::TreeModelColumn<GameObject *> game_obj_ptr;
  };

  ModelColumns columns;

  Gtk::ScrolledWindow scrolled_window;
  Gtk::TreeView tree_view;
  Glib::RefPtr<Gtk::TreeStore> ref_tree_store;
  Glib::RefPtr<Gtk::TreeModel> ref_tree_model;
  Glib::RefPtr<Gtk::TreeSelection> ref_tree_selection;
  Glib::RefPtr<Gtk::GestureClick> right_click_gesture;
  Gtk::Popover context_menu;

  void create_game_object(const std::string &name,
                          const Gtk::TreeModel::iterator parent_itr = {});
  void show_context_menu(double x, double y, bool on_item);

  void on_selection_changed();
  void on_button_press(int n_press, double x, double y);

private:
  std::vector<std::unique_ptr<GameObject>> game_objects;
  int object_counter = 0;
};

#endif // CRAFTR_SCENE_HIERARCHY
