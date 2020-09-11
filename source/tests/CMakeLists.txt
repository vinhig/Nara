cmake_minimum_required(VERSION 3.16)
project(Tests)

add_executable(TArray TArray.cpp)
add_test(Array TArray)

add_executable(TEcs TEcs.cpp ../logic/CTransform.cpp)
add_test(ECS TEcs)