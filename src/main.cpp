#include <glad/glad.h>
#include <iostream>
#include <logger.hpp>

#include <gtkmm/application.h>
#include <gtkmm/notebook.h>
#include <gtkmm/paned.h>
#include <gtkmm/window.h>

#include <craftrEditor/log_display.hpp>

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

    bottom_right.append_page(log_display.main_box);

    set_child(hor);
    log_display.log("application initialized", INFO);
    log_display.log("application initialized", DEBUG);
    log_display.log("application initialized", WARNING);
    log_display.log("application initialized", ERROR);
  };
  ~MainWindow() override {};

protected:
  Gtk::Notebook top_left;
  Gtk::Notebook top_right;
  Gtk::Notebook bottom_left;
  Gtk::Notebook bottom_right;

  LogDisplay log_display;

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
  auto app = std::make_shared<CraftrEditorAppTemp>();
  return app->run(argc, argv);
}
