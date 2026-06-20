#------------------------------
# JustEngine CMake Build System
#------------------------------

cmake_minimum_required(VERSION 3.28)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

project(JustEngine
    VERSION 0.0.1
    LANGUAGES CXX
)

option(JE_USE_SHARED "Build JustEngine as shared library (.dll/.so)." FALSE)

#------------------------------
# Common options
#------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(JE_ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------
# Project setup
#------------------------------

add_executable(${PROJECT_NAME}Exe
    Source/Core/CoreMain.cpp
)

if (JE_USE_SHARED)
    add_library(${PROJECT_NAME} SHARED)
    target_compile_definitions(${PROJECT_NAME} PUBLIC JE_SHARED_LIBRARY)
    target_compile_definitions(${PROJECT_NAME} PRIVATE JE_EXPORT)
else()
    add_library(${PROJECT_NAME} STATIC)
endif()

# TODO: add to separate ide folder (e.x. Tools/)
# ../.clang-format
# ../.gitignore
# ../README.md
set(JE_ENGINE_FILES_PATTERNS
    *.cpp
    *.h
    *.inl
)

set(JE_ENGINE_EXCLUDE_PATTERNS
    .*/CoreMain\\.cpp$
)

set(JE_ENGINE_SOURCES)
je_collect_files(${JE_ENGINE_SOURCE_DIR}
    JE_ENGINE_FILES_PATTERNS
    JE_ENGINE_EXCLUDE_PATTERNS
    JE_ENGINE_SOURCES
)

# TODO: how to optimize it?
set(JE_ENGINE_INCLUDES
    Source/
    Source/Core
)

target_sources(${PROJECT_NAME}
    PRIVATE
    ${JE_ENGINE_SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${JE_ENGINE_INCLUDES}
)

target_link_libraries(${PROJECT_NAME}Exe PRIVATE ${PROJECT_NAME})

#------------------------------
# Target options
#------------------------------

set_target_properties(${PROJECT_NAME}Exe PROPERTIES FOLDER Engine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Engine)

je_create_ide_folders(JE_ENGINE_SOURCES)
je_create_ide_folders(Source/Core/CoreMain.cpp)

# je_setup_pch(${PROJECT_NAME}Exe Source/Core/CorePch.cpp Source/Core/CorePch.cpp Source/Core/CoreMain.cpp)
je_setup_pch(${PROJECT_NAME} Source/Core/CorePch.cpp Source/Core/CorePch.h JE_ENGINE_SOURCES)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /MP /Zc:preprocessor)
    target_compile_options(${PROJECT_NAME}Exe PUBLIC /MP /Zc:preprocessor)
endif()