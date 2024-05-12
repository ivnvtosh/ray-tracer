cmake_minimum_required(VERSION 3.28)

project(RayTracer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# add_definitions("-Wall -Wextra -Werror -std=c++17")
add_definitions("-std=c++17")

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

  ApplicationMacOS/GeneralWindow
  ApplicationMacOS/ProgressWindow
  ApplicationMacOS/RenderWindow

  Core
  Core/Matrix3x3
  Core/Size
  Core/Vector2
  Core/Vector3
  
  Engine
  Engine/Camera
  Engine/Logger
  Engine/OBJImporter
  Engine/PPMExporter
  Engine/RenderShader
  Engine/Shader
  Engine/Time
  Engine/TriangleMesh

  Scene
)

add_executable(ray-tracer
  main.cpp
  Application/Application.cpp
  ApplicationMacOS/ApplicationMacOS.mm
  ApplicationMacOS/MainWindow.mm
  
  ApplicationMacOS/GeneralWindow/GeneralViewController.mm
  ApplicationMacOS/GeneralWindow/GeneralWindow.mm
  
  ApplicationMacOS/ProgressWindow/ProgressViewController.mm
  ApplicationMacOS/ProgressWindow/ProgressWindow.mm
  
  ApplicationMacOS/RenderWindow/RenderViewController.mm
  ApplicationMacOS/RenderWindow/RenderWindow.mm

  Core/Matrix3x3/Matrix3x3.cpp
  Core/Size/Size.cpp
  Core/Vector2/Vector2.cpp
  Core/Vector3/Vector3.cpp

  Engine/Camera/Camera.cpp
  Engine/Logger/Logger.cpp
  Engine/OBJImporter/OBJImporter.cpp
  Engine/PPMExporter/PPMExporter.cpp
  Engine/RenderShader/RenderShader.cpp
  Engine/Shader/Shader.cpp
  Engine/Time/Time.cpp
  
  Scene/Scene.cpp
  Scene/Scene+Events.cpp
)

target_link_libraries(ray-tracer
  ${OPENGL_LIBRARIES}
  ${OpenCL_LIBRARIES}
  "-framework Cocoa"
)
