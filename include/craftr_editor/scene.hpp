#ifndef CRAFTR_SCENE_HPP
#define CRAFTR_SCENE_HPP

#include <glad/glad.h>
#include <gtkmm/box.h>
#include <gtkmm/glarea.h>

#include "logger.hpp"
#include "craftr_engine/scene_graph.hpp"
#include "craftr_engine/game_object.hpp"
#include "craftr_engine/sprite_renderer.hpp"

class Scene {

protected:
  Gtk::GLArea scene_view;

protected:
  void on_realize();
  void on_unrealize();
  bool on_render(const Glib::RefPtr<Gdk::GLContext> &);
  bool on_update();

  // GL helpers
  GLuint compile_shader(GLenum type, const char* src);
  GLuint create_program(const char* vs_src, const char* fs_src);
  void ensure_gl_loaded();

  // GL resources
  GLuint vao = 0;
  GLuint vbo = 0;
  GLuint shader_program = 0;
  GLint uni_transform = -1;
  GLint uni_color = -1;
  bool gl_initialized = false;

public:
  Gtk::Box main_box;
  Scene();
  ~Scene();
};

#endif
