#include "craftr_editor/inspector_panel.hpp"
#include "logger.hpp"

InspectorPanel::InspectorPanel()
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      properties_box(Gtk::Orientation::VERTICAL) {

  set_expand(true);

  // Scrollable area for components
  scrolled_window.set_expand(true);
  scrolled_window.set_policy(Gtk::PolicyType::AUTOMATIC,
                             Gtk::PolicyType::AUTOMATIC);
  scrolled_window.set_child(properties_box);
  append(scrolled_window);

  // Add Component button
  add_component_button.set_label("Add Component");
  add_component_button.signal_clicked().connect(
      sigc::mem_fun(*this, &InspectorPanel::show_add_component_menu));
  append(add_component_button);
}

void InspectorPanel::set_selected_object(GameObject *object) {
  selected_object = object;
  refresh();
}

void InspectorPanel::clear() {
  for (auto *child : properties_box.get_children())
    properties_box.remove(*child);
}

void InspectorPanel::refresh() {
  clear();
  if (!selected_object)
    return;

  // Always show Transform first
  build_component_ui(&selected_object->transform);

  // Display other components
  for (auto &[typeIdx, compPtr] : selected_object->get_all_components()) {
    if (dynamic_cast<Transform *>(compPtr.get()))
      continue;
    build_component_ui(compPtr.get());
  }

  show();
}

void InspectorPanel::build_component_ui(Component *component) {
  auto frame = Gtk::make_managed<Gtk::Frame>(component->get_name());
  auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 4);

  for (auto &prop : component->get_properties()) {
    auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 4);
    auto label = Gtk::make_managed<Gtk::Label>(prop.name);
    label->set_halign(Gtk::Align::START);
    label->set_valign(Gtk::Align::CENTER);
    hbox->append(*label);

    // Different widget types for each PropertyType
    switch (prop.type) {
    case PropertyType::INT: {
      auto adj =
          Gtk::Adjustment::create(std::stoi(prop.getter()), -100000, 100000, 1);
      auto spin = Gtk::make_managed<Gtk::SpinButton>(adj, 1, 0);
      spin->signal_value_changed().connect([&prop, spin]() {
        try {
          prop.setter(std::to_string((int)spin->get_value()));
        } catch (...) {
        }
      });
      hbox->append(*spin);
      break;
    }
    
    case PropertyType::FLOAT: {
      auto initial = std::stof(prop.getter());
      auto adj = Gtk::Adjustment::create(initial, -100000.0, 100000.0, 0.01, 1.0, 0.0);
      auto spin = Gtk::make_managed<Gtk::SpinButton>(adj, 0.01, 4);
      spin->set_digits(4);
      spin->set_numeric(true);
      spin->set_value(initial);

      spin->signal_value_changed().connect([&prop, spin]() {
        try {
          std::ostringstream ss;
          ss.precision(6);
          ss << std::fixed << spin->get_value();
          prop.setter(ss.str());
        } catch (...) {
        }
      });
      hbox->append(*spin);
      break;
    }



    case PropertyType::BOOL: {
      auto check = Gtk::make_managed<Gtk::CheckButton>();
      check->set_active(prop.getter() == "1" || prop.getter() == "true");
      check->signal_toggled().connect(
          [&prop, check]() { prop.setter(check->get_active() ? "1" : "0"); });
      hbox->append(*check);
      break;
    }

    case PropertyType::STRING:
    default: {
      auto entry = Gtk::make_managed<Gtk::Entry>();
      entry->set_text(prop.getter());
      entry->signal_changed().connect(
          [&prop, entry]() { prop.setter(entry->get_text()); });
      hbox->append(*entry);
      break;
    }
    }

    vbox->append(*hbox);
  }

  frame->set_child(*vbox);
  properties_box.append(*frame);
}

void InspectorPanel::show_add_component_menu() {
  if (!selected_object)
    return;

  auto menu_box = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 4);

  for (const auto &entry : ComponentRegistry::instance().get_all()) {
    const std::type_index &typeIdx = entry.first;
    const auto &add_func = entry.second.add_func;
    const auto &comp_name = entry.second.name;

    // Skip Transform
    if (typeIdx == std::type_index(typeid(Transform)))
      continue;

    auto btn = Gtk::make_managed<Gtk::Button>(comp_name);

    btn->signal_clicked().connect([this, typeIdx]() {
      add_component_menu.popdown();
      try {
        ComponentRegistry::instance().add_to_object(typeIdx, *selected_object);
        refresh();
      } catch (const std::exception &e) {
        Logger::instance().log(LogDestination::LOG_WINDOW, e.what(),
                               LogType::ERROR);
      }
    });

    menu_box->append(*btn);
  }

  if (auto old_child = add_component_menu.get_child())
    add_component_menu.unset_child();

  add_component_menu.set_child(*menu_box);
  add_component_menu.set_parent(add_component_button);
  add_component_menu.popup();
}
