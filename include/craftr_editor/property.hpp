#ifndef CRAFTR_PROPERTY_HPP
#define CRAFTR_PROPERTY_HPP

#include <string>
#include <functional>
#include <sstream>
#include <type_traits>
#include <stdexcept>

enum class PropertyType {
    STRING,
    INT,
    FLOAT,
    BOOL
};

struct Property {
    std::string name;
    std::function<std::string()> getter;
    std::function<void(const std::string&)> setter;
    PropertyType type;
};

// Generic type deduction helper
template<typename T>
constexpr PropertyType deduce_property_type() {
    if constexpr (std::is_same_v<T, int>) return PropertyType::INT;
    else if constexpr (std::is_same_v<T, float> || std::is_same_v<T, double>) return PropertyType::FLOAT;
    else if constexpr (std::is_same_v<T, bool>) return PropertyType::BOOL;
    else return PropertyType::STRING;
}

// New, type-safe REGISTER_PROPERTY
#define REGISTER_PROPERTY(fieldName) \
    properties.push_back(Property{#fieldName, \
        [this]() { \
            std::ostringstream ss; ss << this->fieldName; \
            return ss.str(); \
        }, \
        [this](const std::string& val) { \
            std::istringstream ss(val); \
            ss >> this->fieldName; \
            if (ss.fail()) throw std::invalid_argument("Invalid input for property " #fieldName); \
        }, \
        deduce_property_type<decltype(this->fieldName)>() \
    });

#endif