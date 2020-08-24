cmake_minimum_required(VERSION 3.16)
project(Tests)

add_executable(TArray TArray.cpp ../common/Array.cpp)
add_test(Array TArray)