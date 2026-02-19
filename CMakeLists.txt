#------------------------------
# JustEngine CMake Build System
#------------------------------

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(JustEngine
    VERSION 0.0.1
    LANGUAGES CXX
)

#------------------------------
# Common options
#------------------------------

set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
# TODO: doesn't work for Rider
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    PROPERTY
    VS_STARTUP_PROJECT ${PROJECT_NAME}
)

#------------------------------
# Directory options
#------------------------------

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set(CMAKE_PDB_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Binaries)

set(ENGINE_SOURCES Source/main.cpp)
set(ENGINE_INCLUDES)

# TODO: doesn't work for Rider
source_group("Source"
    FILES
    ${ENGINE_SOURCES}
    ${ENGINE_INCLUDES}
)

add_executable(${PROJECT_NAME}
    ${ENGINE_SOURCES}
    ${ENGINE_INCLUDES}
)

#------------------------------
# Target options
#------------------------------

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

if (MSVS)
    target_compile_options(${PROJECT_NAME} ${CMAKE_CXX_FLAGS} "/MP")
endif()