#ifndef CRAFTR_SCENE_HPP
#define CRAFT_SCENE_HPP

#include <gtkmm\box.h>
#include <gtkmm\glarea.h>
#include <glad\glad.h>

#include "logger.hpp"

class Scene{
    
    protected:
        Gtk::GLArea scene_view;

    protected:
        void on_realize();
        void on_unrealize();
        bool on_render(const Glib::RefPtr<Gdk::GLContext>&);
    public:
        Gtk::Box main_box;
        Scene();
        ~Scene();
};


#endif