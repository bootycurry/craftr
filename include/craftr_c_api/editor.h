#ifndef CRAFTR_C_API_EDITOR_H
#define CRAFTR_C_API_EDITOR_H

#include "../craftr_export.h"

#ifdef __cplusplus
extern "C" {
#endif

// Define the function pointer type
typedef void (*SceneRefreshCallback)();

// Store the callback globally
CRAFTR_API void craftr_set_scene_refresh_callback(SceneRefreshCallback cb);

// Call it manually (Python side can invoke this)
CRAFTR_API void craftr_trigger_scene_refresh();

#ifdef __cplusplus
}
#endif

#endif
