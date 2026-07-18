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
# Project compile/link options
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

je_setup_pch(${PROJECT_NAME} Source/Core/CorePch.cpp Source/Core/CorePch.h JE_ENGINE_SOURCES)

target_compile_definitions(${PROJECT_NAME} PUBLIC
    $<$<CONFIG:Debug>:_DEBUG>
    $<$<CONFIG:Debug>:JE_DEBUG>
    
    $<$<CONFIG:Release>:NDEBUG>
    $<$<CONFIG:Release>:JE_RELEASE>
)

if (JE_CXX_MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /MP /Zc:preprocessor)
    target_compile_options(${PROJECT_NAME}Exe PUBLIC /MP /Zc:preprocessor)
elseif (JE_CXX_CLANG)
    # -rdynamic: for linux-specific stacktrace
    target_compile_options(${PROJECT_NAME} PUBLIC -g -std=c++23 -stdlib=libstdc++ -fno-omit-frame-pointer -rdynamic)
    target_compile_options(${PROJECT_NAME}Exe PUBLIC -g -std=c++23 -stdlib=libstdc++ -fno-omit-frame-pointer -rdynamic)
endif()

#------------------------------
# Libraries linking
#------------------------------

# Stacktrace helpers
if (WIN32)
    target_link_libraries(${PROJECT_NAME} PUBLIC dbghelp)
elseif (LINUX)
    target_link_libraries(${PROJECT_NAME} PUBLIC dl pthread)
endif()

# GLFW
je_setup_thirdparty(${PROJECT_NAME} ${JE_THIRDPARTY_DIR}/glfw-3.4/lib/glfw3.lib ${JE_THIRDPARTY_DIR}/glfw-3.4/include)
# OpenGL
target_link_libraries(${PROJECT_NAME} PRIVATE opengl32.lib)

target_link_libraries(${PROJECT_NAME}Exe PRIVATE ${PROJECT_NAME})

#------------------------------
# Project folder structure
#------------------------------

set_target_properties(${PROJECT_NAME}Exe PROPERTIES FOLDER Engine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Engine)

je_create_ide_folders(JE_ENGINE_SOURCES)
je_create_ide_folders(Source/Core/CoreMain.cpp)