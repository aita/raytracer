cmake_minimum_required(VERSION 3.10)

project(raytracer VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

include(ExternalProject)
ExternalProject_Add(stb
  GIT_REPOSITORY    git@github.com:nothings/stb.git
  GIT_TAG           f54acd4e13430c5122cab4ca657705c84aa61b08
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""    
)
ExternalProject_Get_Property(stb SOURCE_DIR)
set(STB_INCLUDE_DIRS ${SOURCE_DIR})
include_directories(${STB_INCLUDE_DIRS})

ExternalProject_Add(glm
  GIT_REPOSITORY    git@github.com:g-truc/glm.git
  GIT_TAG           0.9.9.7 
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""    
)
ExternalProject_Get_Property(glm SOURCE_DIR)
set(GLM_INCLUDE_DIRS ${SOURCE_DIR})
include_directories(${GLM_INCLUDE_DIRS})

add_executable(raytracer main.cc)
add_dependencies(raytracer stb)
add_dependencies(raytracer glm)
target_include_directories(raytracer PUBLIC "${PROJECT_BINARY_DIR}")
