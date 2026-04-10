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

#------------------------------
# Common options
#------------------------------

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(JE_ENGINE_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})

#------------------------------
# Project setup
#------------------------------

add_executable(${PROJECT_NAME}App
    Source/Core/CoreMain.cpp
)

add_library(${PROJECT_NAME}
    STATIC
)

set(JE_ENGINE_SOURCES
    # TODO: add to separate ide folder (e.x. Tools/)
    # ../.clang-format
    # ../.gitignore
    # ../README.md
    # CMakeLists.txt
    Source/Core/CoreDefines.h
    Source/Core/CorePch.h
    Source/Core/CorePch.cpp
    Source/Core/Types/BaseTypes.h
    Source/Core/Types/NonCopyable.h
    Source/Core/Log/LogMisc.cpp
    Source/Core/Log/LogMisc.h
    Source/Core/Log/Logger.cpp
    Source/Core/Log/Logger.h
    Source/Core/Log/LoggerImpl.cpp
    Source/Core/Log/LoggerImpl.h
    Source/Core/Log/Impls/ConsoleLoggerImpl.h
    Source/Core/Log/Impls/ConsoleLoggerImpl.cpp
    Source/Core/Log/Impls/FileLoggerImpl.h
    Source/Core/Log/Impls/FileLoggerImpl.cpp
    Source/Core/Log/LogManager.cpp
    Source/Core/Log/LogManager.h
    Source/Core/Log/LogMacros.h
    Source/Core/Utilities/StringUtilities.h
    Source/Core/Utilities/StringUtilities.cpp
)

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

target_link_libraries(${PROJECT_NAME}App ${PROJECT_NAME})

#------------------------------
# Target options
#------------------------------

set_target_properties(${PROJECT_NAME}App PROPERTIES FOLDER Engine)
set_target_properties(${PROJECT_NAME} PROPERTIES FOLDER Engine)

je_create_ide_folders(JE_ENGINE_SOURCES)
je_create_ide_folders(Source/Core/CoreMain.cpp)

# je_setup_pch(${PROJECT_NAME}App Source/Core/CorePch.cpp Source/Core/CorePch.cpp Source/Core/CoreMain.cpp)
je_setup_pch(${PROJECT_NAME} Source/Core/CorePch.cpp Source/Core/CorePch.cpp JE_ENGINE_SOURCES)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC ${CMAKE_CXX_FLAGS} /MP /Zc:preprocessor)
    target_compile_options(${PROJECT_NAME}App PUBLIC ${CMAKE_CXX_FLAGS} /MP /Zc:preprocessor)
endif()