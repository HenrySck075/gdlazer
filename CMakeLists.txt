cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(osulazer VERSION 1.0.0)

# Set up the mod binary
add_library(${PROJECT_NAME} SHARED
    src/main.cpp
    # Add any extra C++ source files here
    src/intro/IntroTriangles.hpp src/intro/IntroTriangles.cpp
    src/utils.cpp src/utils.hpp  
    # src/LabelBMFont.cpp 

    src/helpers/CustomActions.cpp src/helpers/CustomActions.hpp 
    src/helpers/MouseEvent.hpp src/helpers/MouseEvent.cpp
    src/helpers/colors.hpp

    src/ui/Dialog.hpp src/ui/Dialog.cpp
    src/ui/DialogButton.cpp src/ui/DialogButton.hpp

    src/ui/deco/Triangles.cpp src/ui/deco/Triangles.hpp 
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})
