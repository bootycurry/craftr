#ifndef CRAFTR_SCENE_GRAPH_HPP
#define CRAFTR_SCENE_GRAPH_HPP

#include <vector>
#include <mutex>
#include <algorithm>

class GameObject;

class SceneGraph {
public:
  static SceneGraph& instance() {
    static SceneGraph inst;
    return inst;
  }

  // Register a GameObject for iteration/rendering. It does not take ownership.
  void register_game_object(GameObject* go) {
    std::lock_guard<std::mutex> lg(mutex_);
    objects_.push_back(go);
  }

  // unregister when destroying
  void unregister_game_object(GameObject* go) {
    std::lock_guard<std::mutex> lg(mutex_);
    objects_.erase(std::remove(objects_.begin(), objects_.end(), go),
                   objects_.end());
  }

  std::vector<GameObject*> get_all() {
    std::lock_guard<std::mutex> lg(mutex_);
    return objects_;
  }

private:
  SceneGraph() = default;
  ~SceneGraph() = default;
  std::vector<GameObject*> objects_;
  std::mutex mutex_;
};

#endif
