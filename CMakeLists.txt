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

set(CMAKE_SUPPRESS_REGENERATION ON)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
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

set(ENGINE_SOURCES
    .clang-format
    .gitignore
    CMakeLists.txt
    README.md
    Source/Core/CoreMain.cpp
    Source/Core/CoreDefines.h
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

set(ENGINE_INCLUDES
    Source/
    Source/Core
)

source_group(TREE Source)

add_executable(${PROJECT_NAME}
    ${ENGINE_SOURCES}
)

#------------------------------
# Target options
#------------------------------

target_include_directories(${PROJECT_NAME}
    PUBLIC
    ${ENGINE_INCLUDES}
)

if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC ${CMAKE_CXX_FLAGS} /MP /Zc:preprocessor)
endif()