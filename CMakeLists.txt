cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(pulsebind VERSION 0.0.1)

add_subdirectory(pulsebind)

add_executable(pulsebind_main exec/main.cpp)
target_link_libraries(pulsebind_main PUBLIC pulsebind)

add_subdirectory(test)
