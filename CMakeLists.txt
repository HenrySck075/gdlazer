cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if ("${CMAKE_SYSTEM_NAME}" STREQUAL "iOS" OR IOS)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
else()
    set(CMAKE_OSX_ARCHITECTURES "arm64;x86_64")
endif()
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_ORIGIN_TPL TRUE)

project(gdlazer VERSION 1.0.0)

# ============================================================================
# Skia Setup
# ============================================================================

include(FetchContent)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)



# Map GEODE_TARGET_PLATFORM to Skia platform name
if(GEODE_TARGET_PLATFORM STREQUAL "Win64")
    set(SKIA_PLATFORM "Windows")
    set(SKIA_ARCHIVE "skia-Windows-bin.zip")
elseif(GEODE_TARGET_PLATFORM STREQUAL "Android64")
    set(SKIA_PLATFORM "Android64")
    set(SKIA_ARCHIVE "skia-Android64-bin.tar.gz")
elseif(GEODE_TARGET_PLATFORM STREQUAL "MacOS")
    set(SKIA_PLATFORM "macOS")
    set(SKIA_ARCHIVE "skia-macOS-bin.tar.gz")
else()
    message(WARNING "Skia not available for platform ${GEODE_TARGET_PLATFORM}")
endif()

if(DEFINED SKIA_PLATFORM)
    set(SKIA_VERSION "v5b6a9ad-20251218-084354")
    set(SKIA_URL "https://github.com/HenrySck075/geode-skia/releases/download/${SKIA_VERSION}/${SKIA_ARCHIVE}")
    set(SKIA_HEADERS_URL "https://github.com/HenrySck075/geode-skia/releases/download/${SKIA_VERSION}/skia-headers.tar.gz")
    set(SKIA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/skia/${SKIA_PLATFORM}")
    
    # Download and extract Skia if not already present
    if(NOT EXISTS "${SKIA_DIR}/skia")
        message(STATUS "Downloading Skia for ${SKIA_PLATFORM}...")
        
        # Create temp directory for download
        set(SKIA_TEMP "${CMAKE_CURRENT_BINARY_DIR}/skia_temp")
        file(MAKE_DIRECTORY "${SKIA_TEMP}")
        
        # Download
        # Check if the archive exists first
        if(NOT EXISTS "${SKIA_TEMP}/${SKIA_ARCHIVE}")
            file(DOWNLOAD "${SKIA_URL}" "${SKIA_TEMP}/${SKIA_ARCHIVE}" 
             SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
            
            list(GET DOWNLOAD_STATUS 0 DOWNLOAD_STATUS_CODE)
            if(NOT DOWNLOAD_STATUS_CODE EQUAL 0)
            list(GET DOWNLOAD_STATUS 1 DOWNLOAD_STATUS_MSG)
            file(REMOVE "${SKIA_TEMP}/${SKIA_ARCHIVE}")
            message(FATAL_ERROR "Failed to download Skia binaries: ${DOWNLOAD_STATUS_MSG}")
            endif()
        else()
            message(STATUS "Skia binary archive already exists, skipping download")
        endif()
        if(NOT EXISTS "${SKIA_TEMP}/skia-headers.tar.gz")
          file(DOWNLOAD "${SKIA_HEADERS_URL}" "${SKIA_TEMP}/skia-headers.tar.gz" 
             SHOW_PROGRESS STATUS DOWNLOAD_STATUS)
            
            list(GET DOWNLOAD_STATUS 0 DOWNLOAD_STATUS_CODE)
            if(NOT DOWNLOAD_STATUS_CODE EQUAL 0)
            list(GET DOWNLOAD_STATUS 1 DOWNLOAD_STATUS_MSG)
            file(REMOVE "${SKIA_TEMP}/skia-headers.tar.gz")
            message(FATAL_ERROR "Failed to download Skia headers: ${DOWNLOAD_STATUS_MSG}")
            endif()
        else()
            message(STATUS "Skia headers archive already exists, skipping download")
        endif()
        
        # Extract
        message(STATUS "Extracting Skia...")
        file(MAKE_DIRECTORY "${SKIA_TEMP}/unzipped")
        if(SKIA_ARCHIVE MATCHES "\\.zip$")
            execute_process(COMMAND ${CMAKE_COMMAND} -E tar xf "${SKIA_TEMP}/${SKIA_ARCHIVE}"
                          WORKING_DIRECTORY "${SKIA_TEMP}/unzipped"
                          RESULT_VARIABLE EXTRACT_RESULT)
        else()
            # For tar.gz, extract to a subdirectory first
            execute_process(COMMAND tar -xzf "${SKIA_TEMP}/${SKIA_ARCHIVE}"
                          WORKING_DIRECTORY "${SKIA_TEMP}/unzipped"
                          RESULT_VARIABLE EXTRACT_RESULT)
        endif()
        
        if(NOT EXTRACT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to extract Skia binaries")
        endif()
        
        # Move to final location
        file(MAKE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/skia")
        file(RENAME "${SKIA_TEMP}/unzipped" "${SKIA_DIR}")
        file(MAKE_DIRECTORY "${SKIA_TEMP}/unzipped")
        # Move SKIA_DIR/include to SKIA_DIR/skia/include
        if(SKIA_ARCHIVE MATCHES "\\.zip$")
            execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${SKIA_TEMP}/skia-headers.tar.gz"
                          WORKING_DIRECTORY "${SKIA_TEMP}/unzipped"
                          RESULT_VARIABLE EXTRACT_RESULT)
        else()
            # For tar.gz, extract to a subdirectory first
            execute_process(COMMAND tar -xzf "${SKIA_TEMP}/skia-headers.tar.gz"
                          WORKING_DIRECTORY "${SKIA_TEMP}/unzipped"
                          RESULT_VARIABLE EXTRACT_RESULT)
        endif()
        
        if(NOT EXTRACT_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to extract Skia headers: ${EXTRACT_RESULT}")
        endif()
        
        file(RENAME "${SKIA_TEMP}/unzipped" "${SKIA_DIR}/skia")

        file(REMOVE_RECURSE "${SKIA_TEMP}")
        
        message(STATUS "Skia extracted to ${SKIA_DIR}")
        
        # Rewrite Skia include paths to add skia/ prefix
        set(PYTHON_FOUND FALSE)
        foreach(PYTHON_ALIAS python3 python python3.exe)
            find_program(PYTHON_EXECUTABLE NAMES ${PYTHON_ALIAS})
            if(PYTHON_EXECUTABLE)
            set(PYTHON_FOUND TRUE)
            break()
            endif()
            unset(PYTHON_EXECUTABLE CACHE)
        endforeach()
        
        if(NOT PYTHON_FOUND)
            message(FATAL_ERROR "Python not found! Please install Python 3")
        endif()
        
        execute_process(COMMAND ${PYTHON_EXECUTABLE} "${CMAKE_CURRENT_SOURCE_DIR}/rewrite_skia_includes.py" "${SKIA_DIR}/skia/include"
                  RESULT_VARIABLE REWRITE_RESULT)
        if(NOT REWRITE_RESULT EQUAL 0)
            message(FATAL_ERROR "Failed to rewrite Skia includes")
        endif()
    else()
        message(STATUS "Using cached Skia at ${SKIA_DIR}")
    endif()
endif()

# ============================================================================
# Main Project Setup
# ============================================================================

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS src/*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

# Add Skia support
if(DEFINED SKIA_PLATFORM)
  target_include_directories(${PROJECT_NAME} PUBLIC "${SKIA_DIR}")
  if (SKIA_PLATFORM STREQUAL "Windows")
    file(GLOB_RECURSE SKIA_LIBS "${SKIA_DIR}/lib/*.lib") # crime
  else()
    file(GLOB_RECURSE SKIA_LIBS "${SKIA_DIR}/lib/*") # crime
  endif()
  target_link_libraries(${PROJECT_NAME} ${SKIA_LIBS})
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  target_compile_definitions(${PROJECT_NAME} PUBLIC GDF_DEBUG)
endif()

setup_geode_mod(${PROJECT_NAME})
