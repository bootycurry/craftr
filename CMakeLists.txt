cmake_minimum_required(VERSION 3.14)
project(craftr)

if(MSVC)
  message(
    FATAL_ERROR
      "Use the MinGW toolchain by opening the MSYS2 MinGW UCRT x64 shell for gtkmm."
  )
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-4.0 REQUIRED)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})
add_definitions(${GTKMM_CFLAGS_OTHER})

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GLFW from the external/glfw submodule
add_subdirectory(external/glfw)

# Compile GLAD as a static library
add_library(glad STATIC glad.cpp)
target_include_directories(glad PUBLIC include)

# Platform-specific OpenGL and system libraries
if(UNIX AND NOT APPLE) # Linux
  find_package(X11 REQUIRED)
  find_package(OpenGL REQUIRED)
elseif(APPLE) # macOS
  find_package(OpenGL REQUIRED)
  find_library(COCOA_LIBRARY Cocoa)
  find_library(IOKIT_LIBRARY IOKit)
  find_library(COREVIDEO_LIBRARY CoreVideo)
else() # Windows
  find_package(OpenGL REQUIRED)
endif()

# Main executable
add_executable(
  ${PROJECT_NAME}
  src/main.cpp
  src/logger.cpp
  src/craftr_engine/component.cpp
  src/craftr_engine/game_object.cpp
  src/craftr_engine/transform.cpp
  src/craftr_engine/vector2.cpp
  src/craftr_editor/log_display.cpp
  src/craftr_editor/scene.cpp
  src/craftr_editor/scene_hierarchy.cpp
  src/craftr_editor/inspector_panel.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link libraries
target_link_libraries(
  ${PROJECT_NAME}
  PRIVATE glfw # GLFW submodule (handles window/input)
          glad # GLAD OpenGL loader
          OpenGL::GL # OpenGL
          ${GTKMM_LIBRARIES})

# Platform-specific libraries
if(UNIX AND NOT APPLE) # Linux
  target_link_libraries(${PROJECT_NAME} PRIVATE X11 Xrandr Xi Xxf86vm Xinerama
                                                Xcursor)
elseif(APPLE) # macOS
  target_link_libraries(
    ${PROJECT_NAME} PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY}
                            ${COREVIDEO_LIBRARY})
endif()
