cmake_minimum_required(VERSION 3.16)
project(mochios VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_COMPILER "g++")

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release 
    CACHE STRING 
      "Choose the type of build (Debug or Release)" FORCE)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
option(USE_INSTALL_RESOURCE_PATH "Set resource path to install location" OFF)

file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE LIB_SOURCES "lib/**/src/*.cpp")

add_executable(mochios
  ./example/main.cpp

  ${SOURCES}
  ${LIB_SOURCES}
)

target_include_directories(mochios
  PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:lib/logger/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/logger/include>
    $<INSTALL_INTERFACE:lib/brewtils/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/brewtils/include>
    $<INSTALL_INTERFACE:lib/brewtils/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/nexus/include>
    $<INSTALL_INTERFACE:lib/brewtils/include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lib/json/include>
)