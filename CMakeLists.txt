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

if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(JE_CXX_MSVC TRUE)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(JE_CXX_CLANG TRUE)
else()
    message(FATAL_ERROR "C++ compiler is not supported.")
endif()

set(JE_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(JE_TOOLS_DIR ${JE_ROOT_DIR}/Tools)
set(JE_SAVED_DIR ${JE_ROOT_DIR}/Saved)
set(JE_BINARY_DIR ${CMAKE_BINARY_DIR}/Binaries)
set(JE_THIRDPARTY_DIR ${JE_ROOT_DIR}/ThirdParty)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${JE_BINARY_DIR})
set(CMAKE_PDB_OUTPUT_DIRECTORY ${JE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${JE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${JE_BINARY_DIR})

#------------------------------
# Project setup
#------------------------------

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${JE_THIRDPARTY_DIR}/GLFW)
set_target_properties(glfw PROPERTIES FOLDER ThirdParty)

include(${JE_TOOLS_DIR}/CMake/cmake_helpers.cmake)

add_subdirectory(Engine)
set_property(DIRECTORY ${JE_ROOT_DIR} PROPERTY VS_STARTUP_PROJECT JustEngineExe)

je_system_info()