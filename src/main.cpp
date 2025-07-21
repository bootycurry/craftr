#include <iostream>
#include <glad/glad.h>
#include <logger.hpp>



#include <gtkmm/window.h>
#include <gtkmm/application.h>
#include <craftrEditor/log_window.hpp>

Gtk::Window* temp_main_window = nullptr;
Gtk::Window* log_window = nullptr;

class CraftrEditorAppTemp : public Gtk::Application
{
  protected:
    void on_activate() override {
      // Create your windows and keep them alive as member variables
      main_win = std::make_unique<Gtk::Window>();
      log_win  = std::make_unique<LogWindow>();
      main_win->set_title("Main Window");
      main_win->set_default_size(800, 600);

      add_window(*main_win);
      add_window(*log_win);

      main_win->show();
      log_win->show();
  }

  private:
    std::unique_ptr<Gtk::Window> main_win;
    std::unique_ptr<LogWindow> log_win;
};

int main(int argc, char* argv[])
{
  auto app = std::make_shared<CraftrEditorAppTemp>();
  return app->run(argc, argv);
  
  
  
}