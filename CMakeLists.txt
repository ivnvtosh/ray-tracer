cmake_minimum_required(VERSION 3.28)

project(RayTracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions("-Wall -Wextra -Werror -std=c++17")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

include_directories(
  Engine
  OBJImporter
  PPMExporter
  Render
)

add_executable(ray-tracer
  main.cpp
  OBJImporter/OBJImporter.cpp
  PPMExporter/PPMExporter.cpp
  Render/Render.cpp
)
