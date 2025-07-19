cmake_minimum_required(VERSION 3.14)
project(craftr)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include GLFW from the external/glfw submodule
add_subdirectory(external/glfw)

# Compile GLAD as a static library
add_library(glad STATIC glad.cpp)
target_include_directories(glad PUBLIC include)

# Platform-specific OpenGL and system libraries
if(UNIX AND NOT APPLE)  # Linux
    find_package(X11 REQUIRED)
    find_package(OpenGL REQUIRED)
elseif(APPLE)  # macOS
    find_package(OpenGL REQUIRED)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREVIDEO_LIBRARY CoreVideo)
else()  # Windows
    find_package(OpenGL REQUIRED)
endif()

# Main executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw       # GLFW submodule (handles window/input)
    glad       # GLAD OpenGL loader
    OpenGL::GL # OpenGL
)

# Platform-specific libraries
if(UNIX AND NOT APPLE)  # Linux
    target_link_libraries(${PROJECT_NAME} PRIVATE
        X11 Xrandr Xi Xxf86vm Xinerama Xcursor
    )
elseif(APPLE)  # macOS
    target_link_libraries(${PROJECT_NAME} PRIVATE
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREVIDEO_LIBRARY}
    )
endif()
