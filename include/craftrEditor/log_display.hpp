#ifndef CRAFTR_LOGDISPLAY_HPP
#define CRAFTR_LOGDISPLAY_HPP


#include <gtkmm/box.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/textview.h>
#include <gtkmm/button.h>
#include <gtkmm/window.h>

class LogDisplay
{
    protected:
        Gtk::ScrolledWindow scroll_container;
        Gtk::TextView log_text;
        Gtk::Button reset_button;
        

    public:
        Gtk::Box main_box;
        LogDisplay();
        ~LogDisplay();
        void log(const Glib::ustring& message, bool test);


};

#endif