#include "craftr_editor/scene_hierarchy.hpp"
#include "craftr_editor/log_display.hpp"
#include "logger.hpp"

SceneHierarchy::SceneHierarchy() : Gtk::Box(Gtk::Orientation::VERTICAL) {
  set_expand(true);

  ref_tree_store = Gtk::TreeStore::create(columns);
  tree_view.set_model(ref_tree_store);

  tree_view.append_column("Game Objects", columns.col_name);

  ref_tree_selection = tree_view.get_selection();
  ref_tree_selection->set_mode(Gtk::SelectionMode::SINGLE);
  ref_tree_selection->signal_changed().connect(
      sigc::mem_fun(*this, &SceneHierarchy::on_selection_changed));

  scrolled_window.set_child(tree_view);
  scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC,
                             Gtk::PolicyType::AUTOMATIC);
  scrolled_window.set_expand(true);
  append(scrolled_window);

  context_menu.set_parent(tree_view);

  right_click_gesture = Gtk::GestureClick::create();
  right_click_gesture->set_button(GDK_BUTTON_SECONDARY);
  right_click_gesture->signal_pressed().connect(
      sigc::mem_fun(*this, &SceneHierarchy::on_button_press));
  tree_view.add_controller(right_click_gesture);
}

void SceneHierarchy::on_button_press(int n_press, double x, double y) {
  Gtk::TreeModel::Path path;
  int relative_x = 0, relative_y = 0;
  tree_view.convert_widget_to_bin_window_coords((int)x, (int)y, relative_x,
                                                relative_y);
  if (tree_view.get_path_at_pos(relative_x, relative_y, path)) {
    ref_tree_selection->select(path);
    show_context_menu(x, y, true);
  } else {
    ref_tree_selection->unselect_all();
    show_context_menu(x, y, false);
  }
}

void SceneHierarchy::show_context_menu(double x, double y, bool on_item) {
  auto menu = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);

  if (on_item) {
    auto button = Gtk::make_managed<Gtk::Button>("Create Child Object");
    button->signal_clicked().connect([this]() {
      context_menu.popdown(); // Close the menu
      if (auto iter = ref_tree_selection->get_selected()) {
        create_game_object("GameObject " + std::to_string(object_counter++),
                           iter);
      }
    });
    menu->append(*button);
  } else {
    auto button_new = Gtk::make_managed<Gtk::Button>("New Game Object");
    button_new->signal_clicked().connect([this]() {
      context_menu.popdown();
      create_game_object("GameObject " + std::to_string(object_counter++));
    });
    menu->append(*button_new);

    auto button_collapse = Gtk::make_managed<Gtk::Button>("Collapse All");
    button_collapse->signal_clicked().connect([this]() {
      context_menu.popdown();
      tree_view.collapse_all();
    });
    menu->append(*button_collapse);
  }
  if (auto old_child = context_menu.get_child()) {
    context_menu.unset_child();
  }

  context_menu.set_child(*menu);
  context_menu.set_pointing_to(Gdk::Rectangle(x, y, 1, 1));
  context_menu.popup();
}

void SceneHierarchy::on_selection_changed() {
  auto iter = ref_tree_selection->get_selected();
  if (iter) {
    auto row = *iter;
    GameObject *go = row[columns.game_obj_ptr];
    if (go) {
      Glib::ustring name = row[columns.col_name];
      Logger::instance().log(LogDestination::LOG_WINDOW, name, LogType::INFO);
    }
  }
}

void SceneHierarchy::create_game_object(
    const std::string &name, const Gtk::TreeModel::iterator parent_iter) {

  auto game_obj_owner = std::make_unique<GameObject>();
  GameObject *game_obj_raw_ptr = game_obj_owner.get();

  game_objects.push_back(std::move(game_obj_owner));

  Gtk::TreeModel::iterator new_row_iter;

  if (parent_iter) {
    auto parent_row = *parent_iter;
    GameObject *parent_go = parent_row[columns.game_obj_ptr];

    if (parent_go) {
      game_obj_raw_ptr->set_parent(*parent_go);
      new_row_iter = ref_tree_store->append(parent_iter->children());
      Gtk::TreeModel::Path parent_path = ref_tree_store->get_path(parent_iter);
      tree_view.expand_to_path(parent_path);
    }
  } else {
    new_row_iter = ref_tree_store->append();
  }

  (*new_row_iter)[columns.col_name] = name;
  (*new_row_iter)[columns.game_obj_ptr] = game_obj_raw_ptr;
}

void SceneHierarchy::set_inspector_panel(InspectorPanel &inspector) {
  ref_tree_selection->signal_changed().connect([&]() {
    if (auto iter = ref_tree_selection->get_selected()) {
      GameObject *go = (*iter)[columns.game_obj_ptr];
      inspector.set_selected_object(go);
    } else {
      inspector.set_selected_object(nullptr);
    }
  });
}
