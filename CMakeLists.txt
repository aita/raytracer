cmake_minimum_required(VERSION 3.10)

project(raytracer VERSION 0.1)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


include(ExternalProject)
ExternalProject_Add(stb
  GIT_REPOSITORY    http://github.com/nothings/stb.git
  GIT_TAG           f54acd4e13430c5122cab4ca657705c84aa61b08
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""    
)
ExternalProject_Get_Property(stb SOURCE_DIR)
set(STB_INCLUDE_DIR ${SOURCE_DIR})
include_directories(${STB_INCLUDE_DIR})

ExternalProject_Add(Eigen3
  GIT_REPOSITORY    https://gitlab.com/libeigen/eigen.git
  GIT_TAG           3.3.7
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""    
)
ExternalProject_Get_Property(Eigen3 SOURCE_DIR)
set(EIGEN3_INCLUDE_DIR ${SOURCE_DIR})
include_directories(${EIGEN3_INCLUDE_DIR})

configure_file(src/config.h.in config.h)
file(GLOB_RECURSE SOURCES src/*.cc src/*.h)

add_executable(raytracer ${SOURCES})
add_dependencies(raytracer stb)
add_dependencies(raytracer Eigen3)
target_include_directories(raytracer PUBLIC "${PROJECT_BINARY_DIR}")

find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    target_link_libraries(raytracer PUBLIC OpenMP::OpenMP_CXX)
endif()
