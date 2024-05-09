cmake_minimum_required(VERSION 3.28)

project(RayTracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_definitions("-Wall -Wextra -Werror -std=c++17")

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY
  ${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR})

find_package(OpenGL REQUIRED)
find_package(OpenCL REQUIRED)

include_directories(
  ${OPENGL_INCLUDE_DIRS} 
  ${OPENCL_INCLUDE_DIR}
  Application
  ApplicationMacOS
  Engine/Include
  Scene
)

add_executable(ray-tracer
  main.cpp
  Application/Application.cpp
  ApplicationMacOS/ApplicationMacOS.mm
  ApplicationMacOS/MainWindow.mm
  Engine/Source/Camera.cpp
  Engine/Source/Matrix3x3.cpp
  Engine/Source/Logger.cpp
  Engine/Source/Matrix3x3.cpp
  Engine/Source/OBJImporter.cpp
  Engine/Source/PPMExporter.cpp
  Engine/Source/Shader.cpp
  Engine/Source/Time.cpp
  Engine/Source/Vector3.cpp
  Scene/Scene.cpp
  Scene/Scene+Event.cpp
)

target_link_libraries(ray-tracer
  ${OPENGL_LIBRARIES}
  ${OpenCL_LIBRARIES}
  "-framework Cocoa"
)
