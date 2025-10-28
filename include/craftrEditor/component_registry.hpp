#ifndef CRAFTR_COMPONENT_REGISTRY_HPP
#define CRAFTR_COMPONENT_REGISTRY_HPP

#include <functional>
#include <map>
#include <memory>
#include <string>
#include <typeindex>
#include "craftr_engine/game_object.hpp"

#include <string_view>

inline std::string demangle_type_name(const std::type_info& ti) {
    std::string_view name = ti.name();
#if defined(_MSC_VER)
    if (name.starts_with("class ")) name.remove_prefix(6);
    else if (name.starts_with("struct ")) name.remove_prefix(7);
#endif
    return std::string(name);
}


class ComponentRegistry {
public:
  using AddFunc = std::function<void(GameObject&)>;

  struct Entry {
        AddFunc add_func;
        std::string name;
    };

  static ComponentRegistry& instance() {
    static ComponentRegistry inst;
    return inst;
  }

  template<typename T>
  void register_component() {
    const std::type_index idx(typeid(T));

    // prevent duplicates in the registry
    if (entries_.count(idx)) return;

    
    entries_[idx] = Entry{
            [](GameObject& obj) { obj.add_component<T>(); },
            demangle_type_name(typeid(T))
        };
  }

  void add_to_object(std::type_index idx, GameObject& obj) const {
    auto it = entries_.find(idx);
    if (it != entries_.end())
      it->second.add_func(obj);
  }

  const std::map<std::type_index, Entry>& get_all() const { return entries_; }

private:
  std::map<std::type_index, Entry> entries_;
};

#define REGISTER_COMPONENT(T) \
namespace { \
    struct AutoRegister_##T { \
        AutoRegister_##T() { \
            ComponentRegistry::instance().register_component<T>(); \
        } \
    }; \
    static AutoRegister_##T _auto_register_##T; \
}

#endif
