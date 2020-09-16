cmake_minimum_required(VERSION 3.16)
project(Nara)

set(CMAKE_CXX_STANDARD 20)

enable_testing()
add_subdirectory(source/tests)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(Threads REQUIRED)

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
    source/logic/CTransform.cpp source/logic/CTransform.h
    source/logic/CLabel.h
    source/logic/CMeshInstance.cpp source/logic/CMeshInstance.h
    source/logic/CCamera.cpp source/logic/CCamera.h
    source/logic/CTerrain.cpp source/logic/CTerrain.h
    source/logic/Game.cpp source/logic/Game.h
    source/logic/Ecs.h
)

add_library(openfbx
    libraries/openfbx/src/miniz.c libraries/openfbx/src/miniz.h
    libraries/openfbx/src/ofbx.cpp libraries/openfbx/src/ofbx.h
)

add_library(renderer
    source/renderer/Args.h
    source/renderer/Backend.cpp source/renderer/Backend.h
    source/renderer/BackendOgl.cpp source/renderer/BackendOgl.h
    source/renderer/Device.cpp source/renderer/Device.h
    source/renderer/Frame.cpp source/renderer/Frame.h
    source/renderer/Settings.cpp source/renderer/Settings.h
    source/renderer/RenderGraph.h source/renderer/RenderGraph.cpp
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
target_link_libraries(Nara Threads::Threads)