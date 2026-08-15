# Specify the CMake minimum version required to build this project
cmake_minimum_required(VERSION 3.14)

project(ome 
  LANGUAGES CXX 
  VERSION 1.1.1
  DESCRIPTION "An order matching engine written in C++."
)

# GoogleTest requires at least C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Get all headers and sources for OME_CORE. CONFIGURE_DEPENDS makes cmake re-check at build time 
file(GLOB OME_CORE_SOURCES CONFIGURE_DEPENDS "src/*.cpp")

# Create the ome_core library target 
add_library(ome_core STATIC) 
target_sources(ome_core PRIVATE ${OME_CORE_SOURCES})
target_include_directories(ome_core PUBLIC include)

# Create the main executable 
add_executable(ome_main)
target_sources(ome_main PRIVATE apps/main.cpp) 
target_link_libraries(ome_main PRIVATE ome_core) 

# Add the tests subdirectory to create the tests executables 
enable_testing()
add_subdirectory(tests)

