#include "craftr_editor/scene.hpp"
#include <array>
#include <cmath>
#include <string>

Scene::Scene() {
  // request an OpenGL 3.3 core context
  scene_view.set_required_version(3, 3);
  scene_view.set_hexpand(true);
  scene_view.set_vexpand(true);
  scene_view.set_size_request(400, 300); // ensures initial allocation

  scene_view.signal_realize().connect(sigc::mem_fun(*this, &Scene::on_realize));
  scene_view.signal_unrealize().connect(sigc::mem_fun(*this, &Scene::on_unrealize));
  scene_view.signal_render().connect(sigc::mem_fun(*this, &Scene::on_render), false);

  Glib::signal_timeout().connect(sigc::mem_fun(*this, &Scene::on_update), 16);

  main_box.set_hexpand(true);
  main_box.set_vexpand(true);
  main_box.append(scene_view);
}

Scene::~Scene() {
  // destroy GL resources if alive
  if (gl_initialized) {
    if (shader_program) glDeleteProgram(shader_program);
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
  }
}

void Scene::ensure_gl_loaded() {
  if (gl_initialized) return;

  // Make current
  auto ctx = scene_view.get_context();
  if (ctx) {
    ctx->make_current();
  }

  // load GL function pointers using glad
  if (!gladLoadGL()) {
    Logger::instance().log(LogDestination::LOG_WINDOW, Glib::ustring("Failed to load GL via glad"), LogType::ERROR);
    return;
  }

  // Simple square: two triangles, positions only (NDC, we'll scale/translate in vertex shader)
  const float verts[] = {
    // x, y
    -0.5f, -0.5f,
     0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f, -0.5f,
     0.5f,  0.5f,
    -0.5f,  0.5f
  };

  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  // Position attribute
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

  // Compile shaders
  const char* vertex_src = R"(
#version 330 core
layout(location = 0) in vec2 aPos;
uniform vec3 uPos; // x,y,z (z unused, keep for future)
uniform vec2 uScale;
uniform float uRotation; // in degrees
uniform vec2 uViewport; // width,height in pixels (optional)
uniform mat4 uProjection;
void main() {
    // apply scale and rotation to vertex (in local space)
    float rad = radians(uRotation);
    mat2 rot = mat2(cos(rad), -sin(rad), sin(rad), cos(rad));
    vec2 pos = rot * (aPos * uScale);
    // translation
    vec2 world = pos + uPos.xy;
    // convert to clip space (we assume uProjection provided)
    gl_Position = uProjection * vec4(world, 0.0, 1.0);
}
)";

  const char* frag_src = R"(
#version 330 core
out vec4 FragColor;
uniform vec4 uColor;
void main() {
  FragColor = uColor;
}
)";

  shader_program = create_program(vertex_src, frag_src);
  if (!shader_program) {
    Logger::instance().log(LogDestination::LOG_WINDOW, Glib::ustring("Failed to create shader program"), LogType::ERROR);
  } else {
    uni_transform = glGetUniformLocation(shader_program, "uPos");
    uni_color = glGetUniformLocation(shader_program, "uColor");
  }

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);

  gl_initialized = true;
}

void Scene::on_realize() {
  // Ensure GL is initialized when the widget is realized
  ensure_gl_loaded();
}

void Scene::on_unrealize() {
  // nothing currently
}

static float ortho_left = -400.0f;
static float ortho_right = 400.0f;
static float ortho_bottom = -300.0f;
static float ortho_top = 300.0f;

bool Scene::on_render(const Glib::RefPtr<Gdk::GLContext> &) {
  ensure_gl_loaded();
  if (!gl_initialized) return true;

  // Make the GLArea current
  auto ctx = scene_view.get_context();
  if (ctx) ctx->make_current();

  // Set viewport to widget allocation
  int width = scene_view.get_allocated_width();
  int height = scene_view.get_allocated_height();
  glViewport(0, 0, width, height);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glUseProgram(shader_program);
  glBindVertexArray(vao);

  // build a simple orthographic projection that maps world coords to NDC
  // For simplicity we'll use pixel-space mapping: world units ~ pixels with center at (0,0)
  // build projection matrix manually:
  float left = -width/2.0f;
  float right = width/2.0f;
  float bottom = -height/2.0f;
  float top = height/2.0f;
  float near = -1.0f;
  float far = 1.0f;

  float proj[16] = {
    2.0f/(right-left), 0, 0, 0,
    0, 2.0f/(top-bottom), 0, 0,
    0, 0, -2.0f/(far-near), 0,
    -(right+left)/(right-left), -(top+bottom)/(top-bottom), -(far+near)/(far-near), 1.0f
  };

  GLint loc_proj = glGetUniformLocation(shader_program, "uProjection");
  if (loc_proj != -1) {
    glUniformMatrix4fv(loc_proj, 1, GL_FALSE, proj);
  }

  // Iterate over all GameObjects in the scene graph
  auto all_objects = SceneGraph::instance().get_all();
  for (auto go : all_objects) {
    if (!go) continue;
    // check for SpriteRenderer
    SpriteRenderer* sr = go->get_component<SpriteRenderer>();
    if (!sr) continue;

    // use transform values (position, scale, rotation)
    float px = go->transform.position.x;
    float py = go->transform.position.y;
    float sx = go->transform.scale.x;
    float sy = go->transform.scale.y;
    float rot = go->transform.rotation;

    // set uniforms
    GLint loc_uPos = glGetUniformLocation(shader_program, "uPos");
    if (loc_uPos != -1) {
      glUniform3f(loc_uPos, px, py, 0.0f);
    }
    GLint loc_uScale = glGetUniformLocation(shader_program, "uScale");
    if (loc_uScale != -1) {
      glUniform2f(loc_uScale, sx, sy);
    }
    GLint loc_uRot = glGetUniformLocation(shader_program, "uRotation");
    if (loc_uRot != -1) {
      glUniform1f(loc_uRot, rot);
    }
    // color
    GLint loc_uColor = glGetUniformLocation(shader_program, "uColor");
    if (loc_uColor != -1) {
      glUniform4f(loc_uColor, sr->red, sr->green, sr->blue, sr->alpha);
    }

    // Draw the quad
    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  glBindVertexArray(0);
  glUseProgram(0);

  return true;
}

GLuint Scene::compile_shader(GLenum type, const char* src) {
  GLuint shader = glCreateShader(type);
  glShaderSource(shader, 1, &src, nullptr);
  glCompileShader(shader);

  GLint status = 0;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
  if (!status) {
    GLint len = 0;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
    std::string log(len, '\0');
    glGetShaderInfoLog(shader, len, nullptr, &log[0]);
    Logger::instance().log(LogDestination::LOG_WINDOW, Glib::ustring("Shader compilation failed: " + log), LogType::ERROR);
    glDeleteShader(shader);
    return 0;
  }
  return shader;
}

GLuint Scene::create_program(const char* vs_src, const char* fs_src) {
  GLuint vs = compile_shader(GL_VERTEX_SHADER, vs_src);
  if (!vs) return 0;
  GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fs_src);
  if (!fs) { glDeleteShader(vs); return 0; }

  GLuint prog = glCreateProgram();
  glAttachShader(prog, vs);
  glAttachShader(prog, fs);
  glBindAttribLocation(prog, 0, "aPos");
  glLinkProgram(prog);

  GLint status = 0;
  glGetProgramiv(prog, GL_LINK_STATUS, &status);
  if (!status) {
    GLint len = 0;
    glGetProgramiv(prog, GL_INFO_LOG_LENGTH, &len);
    std::string log(len, '\0');
    glGetProgramInfoLog(prog, len, nullptr, &log[0]);
    Logger::instance().log(LogDestination::LOG_WINDOW, Glib::ustring("Program link failed: " + log), LogType::ERROR);
    glDeleteProgram(prog);
    prog = 0;
  }

  glDeleteShader(vs);
  glDeleteShader(fs);
  return prog;
}

bool Scene::on_update() {
    scene_view.queue_draw(); 
    return true; 
}
