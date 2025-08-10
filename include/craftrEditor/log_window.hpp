#ifndef CRAFTER_LOGWINDOW_HPP
#define CRAFTER_LOGWINDOW_HPP

#include <gtkmm/button.h>
#include <gtkmm/frame.h>
#include<gtkmm/window.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/box.h>
#include <vector>
#include <string.h>

class LogWindow : public Gtk::Window
{

public:
  LogWindow();
  ~LogWindow() override;
  void log(const Glib::ustring& message, bool test);

protected:
  //Signal handlers:
  

  //Member widgets:
  Gtk::ScrolledWindow scroll_container;
  Gtk::TextView log_text;
  Gtk::Button reset_button;
  Gtk::Box main_box;

};

#endif

