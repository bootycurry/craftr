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

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
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

# Core Engine Static Library
#======================================================
add_library(craftr_core STATIC
  src/logger.cpp
  src/craftr_engine/component.cpp
  src/craftr_engine/game_object.cpp
  src/craftr_engine/transform.cpp
  src/craftr_engine/vector2.cpp
  src/craftr_editor/log_display.cpp
  src/craftr_editor/scene.cpp
  src/craftr_editor/scene_hierarchy.cpp
  src/craftr_editor/inspector_panel.cpp
  src/craftr_engine/sprite_renderer.cpp
)

# Make headers under include/ available to core and to anyone who links craftr_core
target_include_directories(craftr_core
  PUBLIC
    ${CMAKE_SOURCE_DIR}/include
)

# Link platform and helper libs required by core
# Use PUBLIC so consumers (exe / shared lib) inherit necessary link deps
target_link_libraries(craftr_core
  PUBLIC
    glfw
    glad
    OpenGL::GL
    ${GTKMM_LIBRARIES}
)

# Also link platform-specific system libs to core (so consumers don't need to)
if(UNIX AND NOT APPLE)
  target_link_libraries(craftr_core PUBLIC X11 Xrandr Xi Xxf86vm Xinerama
                        Xcursor)
elseif(APPLE)
  target_link_libraries(craftr_core PUBLIC ${COCOA_LIBRARY} ${IOKIT_LIBRARY}
                        ${COREVIDEO_LIBRARY})
endif()


# Main executable
#======================================================
add_executable(${PROJECT_NAME}
  src/main.cpp
)
# executable needs to see headers as well
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

# Link main executable against core (core already brings glfw/glad/OpenGL/GTK as PUBLIC)
target_link_libraries(${PROJECT_NAME}
  PRIVATE
    craftr_core
    craftr_c_api
    glad
)

# C API shared library (exposes C ABI)
#======================================================
add_library(craftr_c_api SHARED
  src/craftr_c_api/game_object_wrapper.cpp
  src/craftr_c_api/scene_wrapper.cpp
  src/craftr_c_api/editor_wrapper.cpp
)

# Ensure the shared library sees headers
target_include_directories(craftr_c_api
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Link the shared library to the engine core (and helper libs if needed)
target_link_libraries(craftr_c_api
  PRIVATE
    craftr_core
)

# When building the shared library, define macro so export header uses dllexport on Windows
target_compile_definitions(craftr_c_api
  PRIVATE
    CRAFTR_BUILD_DLL
)


# Platform-specific adjustments for the shared library
#======================================================
if(UNIX AND NOT APPLE)
  # on Linux make sure the shared lib also links the same system libraries (if not inherited)
  target_link_libraries(craftr_c_api PRIVATE X11 Xrandr Xi Xxf86vm Xinerama
                        Xcursor)
elseif(APPLE)
  target_link_libraries(craftr_c_api PRIVATE ${COCOA_LIBRARY} ${IOKIT_LIBRARY}
                        ${COREVIDEO_LIBRARY})
endif()
