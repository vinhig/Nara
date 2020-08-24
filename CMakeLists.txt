cmake_minimum_required(VERSION 3.16)
project(Nara)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(source/tests)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

add_library(common
    source/common/Array.h source/common/Array.cpp
    source/common/Macros.h
)

add_library(renderer
    source/renderer/Device.cpp source/renderer/Device.h
    source/renderer/BackendOgl46.cpp source/renderer/BackendOgl46.h
    source/renderer/BackendOgl33.cpp source/renderer/BackendOgl33.h
    source/renderer/Args.h source/renderer/Backend.cpp
    source/renderer/Backend.h
)

add_library(logic
    source/logic/Game.cpp source/logic/Game.h
)

add_executable(Nara
    source/main.cpp
)

target_link_libraries(Nara common)
target_link_libraries(Nara renderer)
target_link_libraries(Nara logic)
target_link_libraries(Nara glfw)
target_link_libraries(Nara OpenGL::GL)
target_link_libraries(Nara GLEW::GLEW)