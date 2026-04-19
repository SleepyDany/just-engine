#------------------------------
# JustEngine CMake Build System
#------------------------------

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(JE
    VERSION 1.0.0
    LANGUAGES CXX
)

#------------------------------
# Common options
#------------------------------

set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(JE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(JE_TOOLS_DIR ${JE_ROOT_DIR}/Tools)
set(JE_SAVED_DIR ${JE_ROOT_DIR}/Saved)
set(JE_BINARY_DIR ${CMAKE_BINARY_DIR}/Binaries)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${JE_BINARY_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${JE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${JE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${JE_BINARY_DIR})

#------------------------------
# Project setup
#------------------------------

include(${JE_TOOLS_DIR}/CMake/cmake_helpers.cmake)

add_subdirectory(Engine)
set_property(DIRECTORY ${JE_ROOT_DIR} PROPERTY VS_STARTUP_PROJECT JustEngineExe)

je_system_info()