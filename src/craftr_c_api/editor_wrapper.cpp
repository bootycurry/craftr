#include "craftr_c_api/editor.h"
#include "logger.hpp"

static SceneRefreshCallback g_scene_refresh_cb = nullptr;

extern "C" {

CRAFTR_API void craftr_set_scene_refresh_callback(SceneRefreshCallback cb) {
  g_scene_refresh_cb = cb;
  Logger::instance().log(CONSOLE, "Scene refresh callback registered", INFO);
}

CRAFTR_API void craftr_trigger_scene_refresh() {
  if (g_scene_refresh_cb) {
    Logger::instance().log(CONSOLE, "Triggering scene refresh...", DEBUG);
    g_scene_refresh_cb();
  } else {
    Logger::instance().log(CONSOLE, "No scene refresh callback set", WARNING);
  }
}
}
