#ifndef CRAFTR_EXPORT_H
#define CRAFTR_EXPORT_H

#if defined(_WIN32) || defined(_WIN64)
  #if defined(CRAFTR_BUILD_DLL)
    #define CRAFTR_API __declspec(dllexport)
  #else
    #define CRAFTR_API __declspec(dllimport)
  #endif
#else
  #if __GNUC__ >= 4
    #define CRAFTR_API __attribute__((visibility("default")))
  #else
    #define CRAFTR_API
  #endif
#endif

#endif // CRAFTR_EXPORT_H
