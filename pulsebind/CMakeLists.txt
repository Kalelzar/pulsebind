cmake_minimum_required(VERSION 3.15)
set(CMAKE_CXX_STANDARD 17)

project(pulsebind VERSION 1.0.0)

file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS
        "*.hpp"
        "*.cpp")

add_library(pulsebind SHARED ${ALL_SRC})
target_link_libraries(pulsebind PUBLIC pulse)

set(HEADERS pulseaudio.hpp
  client.hpp
  list.hpp
  server.hpp
  sink.hpp
  softwareSink.hpp
  source.hpp
  softwareSource.hpp
  device.hpp)

set_target_properties(pulsebind
  PROPERTIES
  LINKER_LANGUAGE CXX
  FRAMEWORK true
  FRAMEWORK_VERSION CXX
  VERSION 1.0.0
  SOVERSION 1
  PUBLIC_HEADER "${HEADERS}")

install(TARGETS pulsebind
  LIBRARY DESTINATION
  PUBLIC_HEADER DESTINATION include/pulsebind)
