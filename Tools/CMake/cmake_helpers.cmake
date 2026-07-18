macro(je_system_info)
    message("")
    message("#---------System info----------#")

    if (WIN32)
        message("OS: Windows")
    elseif (LINUX)
        message("OS: Linux")
    endif()

    message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
    if (JE_CXX_MSVC)
        message("MSVC version: ${MSVC_VERSION}")
        message("MSVC toolset: ${MSVC_TOOLSET_VERSION}")
    elseif (JE_CXX_CLANG)
        message("Clang version: ${CMAKE_CXX_COMPILER_VERSION}")
    endif()

    message("Compiler C++ flags: ${CMAKE_CXX_FLAGS}")
    message("Compiler C++ debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
    message("Compiler C++ release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
    message("Configuration types: ${CMAKE_CONFIGURATION_TYPES}")
    
    message("#------------------------------#")
    message("")
endmacro()

macro(je_collect_files DIR_PATH INCLUDE_PATTERNS EXCLUDE_PATTERNS OUT_FILES)
    foreach(PATTERN IN LISTS ${INCLUDE_PATTERNS})
        list(APPEND WORK_PATTERNS "${DIR_PATH}/${PATTERN}")
    endforeach()

    file(GLOB_RECURSE ${OUT_FILES} CONFIGURE_DEPENDS
        ${WORK_PATTERNS}
    )

    foreach(PATTERN IN LISTS ${EXCLUDE_PATTERNS})
        list(FILTER ${OUT_FILES} EXCLUDE REGEX "${PATTERN}")
    endforeach()
endmacro()

macro(je_create_ide_folders SOURCE_FILES)
    foreach(FILE IN LISTS ${SOURCE_FILES})
        get_filename_component(FILE_PATH ${FILE} PATH)
        string(REPLACE ${PROJECT_SOURCE_DIR} "" FILE_PATH_REL ${FILE_PATH})
        string(REPLACE "/" "\\" GROUP_PATH ${FILE_PATH_REL})
        source_group(${GROUP_PATH} FILES ${FILE})
    endforeach()
endmacro()

function(je_setup_pch TARGET PCH_SOURCE PCH_HEADER SOURCE_FILES)
    # extract pch filename
    get_filename_component(PCH_HEADER_NAME ${PCH_HEADER} NAME)

    # create by /Yc .pch file from PCH_SOURCE
    target_sources(${TARGET} PRIVATE ${PCH_HEADER} ${PCH_SOURCE})

    if (JE_CXX_MSVC)
        set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER_NAME}")
    elseif (JE_CXX_CLANG)
        set(PCH_HEADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PCH_HEADER})
        # TODO: Target.dir/ is default directory for build .obj/.o files...
        set(PCH_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/$<CONFIG>/${PCH_HEADER_NAME}.pch)
        
        # create missing directory and create .pch file with separate command
        add_custom_target(setup_pch_clang
            COMMAND ${CMAKE_COMMAND}
                -E make_directory "${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/$<CONFIG>/"
            COMMAND clang++
                ${CMAKE_CXX_FLAGS}
                -std=c++${CMAKE_CXX_STANDARD}
                -x c++-header ${PCH_HEADER_PATH}
                -o ${PCH_OUTPUT_PATH}
        )

        add_dependencies(${TARGET} setup_pch_clang)
    endif()

    # mark all other .cpp (except PCH_SOURCE) to use .pch file
    foreach(FILE ${SOURCE_FILES})
        if (${FILE} MATCHES "\\.cpp$" AND NOT ${FILE} STREQUAL ${PCH_SOURCE})
            if (MSVC)
                set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yu${PCH_HEADER_NAME}")
            endif()
        endif()
    endforeach()

    # add PCH_HEADER to TARGET includes
    if (JE_CXX_MSVC)
        target_compile_options(${TARGET} PRIVATE "/FI${PCH_HEADER_NAME}")
    elseif (JE_CXX_CLANG)
        target_compile_options(${TARGET} PRIVATE -include ${CMAKE_CURRENT_SOURCE_DIR}/${PCH_HEADER})
    endif()
endfunction()

# TODO:
function(je_setup_thirdparty TARGET LIB_PATH INCLUDE_DIR)
    target_link_libraries(${TARGET} PUBLIC ${LIB_PATH})
    target_include_directories(${TARGET} PUBLIC ${INCLUDE_DIR})
endfunction()