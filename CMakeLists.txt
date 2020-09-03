cmake_minimum_required(VERSION 3.16)
project(Nara)

set(CMAKE_CXX_STANDARD 11)

enable_testing()
add_subdirectory(source/tests)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)

include_directories(libraries/)
include_directories(libraries/openfbx/src/)

add_library(common
   source/common/Array.h
   source/common/Macros.h
   source/common/File.cpp source/common/File.h
)

add_library(loader
    source/loader/DefaultTextureLoader.cpp source/loader/DefaultTextureLoader.h
    source/loader/FbxMeshLoader.cpp source/loader/FbxMeshLoader.h
)

add_library(logic
    source/logic/Game.cpp source/logic/Game.h
)

add_library(openfbx
    libraries/openfbx/src/miniz.c libraries/openfbx/src/miniz.h
    libraries/openfbx/src/ofbx.cpp libraries/openfbx/src/ofbx.h
)

add_library(renderer
    source/renderer/Args.h
    source/renderer/Backend.cpp source/renderer/Backend.h
    source/renderer/BackendOgl33.cpp source/renderer/BackendOgl33.h
    source/renderer/BackendOgl46.cpp source/renderer/BackendOgl46.h
    source/renderer/Device.cpp source/renderer/Device.h
    source/renderer/Frame.cpp source/renderer/Frame.h
)

add_executable(Nara
    source/main.cpp
)

target_link_libraries(Nara common)
target_link_libraries(Nara logic)
target_link_libraries(Nara renderer)
target_link_libraries(Nara loader)
target_link_libraries(Nara openfbx)
target_link_libraries(Nara glfw)
target_link_libraries(Nara OpenGL::GL)
target_link_libraries(Nara GLEW::GLEW)