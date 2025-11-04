#include <glad/glad.h>
#include <gtkmm-4.0/gtkmm.h>

#include "craftr_editor/inspector_panel.hpp"
#include "craftr_editor/log_display.hpp"
#include "craftr_editor/scene_hierarchy.hpp"
#include "glibmm/ustring.h"
#include "logger.hpp"
#include <craftr_editor/scene.hpp>

Gtk::Window *temp_main_window = nullptr;

class MainWindow : public Gtk::Window {
public:
  MainWindow() {
    hor = Gtk::Paned(Gtk::Orientation::VERTICAL);
    top = Gtk::Paned(Gtk::Orientation::HORIZONTAL);
    bot = Gtk::Paned(Gtk::Orientation::HORIZONTAL);

    hor.set_start_child(top);
    hor.set_end_child(bot);
    top.set_start_child(top_left);
    top.set_end_child(top_right);
    bot.set_start_child(bottom_left);
    bot.set_end_child(bottom_right);

    bottom_right.append_page(log_display.main_box, "Log");
    top_right.append_page(scene_hierarchy, "Scene");
    bottom_left.append_page(inspector_panel, "Inspector");
    top_left.append_page(scene.main_box, "Scene View");

    scene_hierarchy.set_inspector_panel(inspector_panel);

    set_child(hor);
    Logger::instance().set_target_display(&log_display);
    Logger::instance().log(LOG_WINDOW, Glib::ustring("Session Started"), INFO);
  };
  ~MainWindow() override {};

protected:
  Gtk::Notebook top_left;
  Gtk::Notebook top_right;
  Gtk::Notebook bottom_left;
  Gtk::Notebook bottom_right;

  LogDisplay log_display;
  SceneHierarchy scene_hierarchy;
  InspectorPanel inspector_panel;
  Scene scene;

private:
  Gtk::Paned hor;
  Gtk::Paned top;
  Gtk::Paned bot;
};

class CraftrEditorAppTemp : public Gtk::Application {
protected:
  void on_activate() override {
    // Create your windows and keep them alive as member variables
    main_win = std::make_unique<MainWindow>();
    main_win->set_title("Main Window");
    main_win->set_default_size(800, 600);

    add_window(*main_win);

    main_win->show();
  }

private:
  std::unique_ptr<MainWindow> main_win;
};

int main(int argc, char *argv[]) {
  Logger::instance().set_file_path(std::string("./logs/"));
  Logger::instance().log(CONSOLE, Glib::ustring("Session Started"), INFO);
  Logger::instance().log(LOG_FILE, Glib::ustring("Session Started"), INFO);
  auto app = std::make_shared<CraftrEditorAppTemp>();
  Glib::RefPtr<Gtk::Settings> settings = Gtk::Settings::get_default();
  settings->property_gtk_application_prefer_dark_theme().set_value(true);
  return app->run(argc, argv);
}
