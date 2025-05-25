cmake_minimum_required(VERSION 3.14)
project(craftr)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)

add_library(glad STATIC glad.c) # compile glad as a static library
target_include_directories(glad PUBLIC include) # include headers while compiling glad

add_executable(${PROJECT_NAME} src/main.c)
target_include_directories(${PROJECT_NAME} PRIVATE include) # include custom headers for project (present in the include folder)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw glad) # link glfw and glad to the executable (private visibility is used as the libraries need not be exposed for external use)
