# TODO: currently theres a discussion about dropping MSVC support so

cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
project(osulazer VERSION 1.0.0)
if("${CMAKE_GENERATOR}" EQUAL "Unix Makefiles")
    target_compile_options(${PROJECT_NAME} PUBLIC -j8)
endif()
add_definitions(-w)
file(GLOB_RECURSE src src/*.cpp)
file(GLOB_RECURSE incl src/*.hpp)
# Set up the mod binary
add_library(${PROJECT_NAME} SHARED
    ${src}
    ${incl}
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
target_compile_definitions(geode-sdk INTERFACE GEODE_EXPOSE_SECRET_INTERNALS_IN_HEADERS_DO_NOT_DEFINE_PLEASE)
# include("${CMAKE_CURRENT_SOURCE_DIR}/src/devtools/CMakeLists.txt")
# setupCustomDevtools()
# Set up dependencies, resources, and link Geode.
setup_geode_mod(${PROJECT_NAME})

#execute_process(COMMAND python ${CMAKE_CURRENT_SOURCE_DIR}/src/game/check_commit.py)
if (0 AND EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/copy_to_framework.py") 
  execute_process(COMMAND python "${CMAKE_CURRENT_SOURCE_DIR}/copy_to_framework.py")
endif()
