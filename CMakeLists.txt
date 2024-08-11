cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)

project(osulazer VERSION 1.0.0)
add_definitions(-w)
# Set up the mod binary
file(GLOB_RECURSE src src/*.cpp src/*.hpp)
add_library(${PROJECT_NAME} SHARED
    ${src}
    # Fill start
    # Fill end
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)
if(MSVC)
  #target_compile_options(${PROJECT_NAME} PUBLIC "/fsanitize=address")
endif()
# android
# target_link_libraries(${PROJECT_NAME} -L/data/data/com.termux/files/usr/lib/libGLESv2.so.2)

# include("${CMAKE_CURRENT_SOURCE_DIR}/src/devtools/CMakeLists.txt")
# setupCustomDevtools()
# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})

#execute_process(COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/src/game/check_commit.py)
