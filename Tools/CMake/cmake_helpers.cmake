macro(je_system_info)
    message("")
    message("#---------System info----------")

    if (WIN32)
        message("OS: Windows")
    elseif (LINUX)
        message("OS: Linux")
    endif()

    message("Compiler: ${CMAKE_CXX_COMPILER_ID}")
    if (MSVC)
        message("MSVC version: ${MSVC_VERSION}")
        message("MSVC toolset: ${MSVC_TOOLSET_VERSION}")
    endif()

    message("Compiler C++ flags: ${CMAKE_CXX_FLAGS}")
    message("Compiler C++ debug flags: ${CMAKE_CXX_FLAGS_DEBUG}")
    message("Compiler C++ release flags: ${CMAKE_CXX_FLAGS_RELEASE}")
    message("Configuration types: ${CMAKE_CONFIGURATION_TYPES}")
    
    message("#------------------------------")
    message("")
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

    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(CLANG TRUE)
    endif()

    # create by /Yc .pch file from PCH_SOURCE
    target_sources(${TARGET} PRIVATE ${PCH_HEADER} ${PCH_SOURCE})
    if (MSVC)
        set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER_NAME}")
    else(CLANG) # Clang
        # set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "-x c++-header ${CMAKE_CURRENT_LIST_DIR}/${PCH_HEADER} -o ${PROJECT_NAME}.pch")
        
        set(PCH_HEADER_PATH ${CMAKE_CURRENT_SOURCE_DIR}/${PCH_HEADER})
        # set(PCH_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/$<CONFIG>/${TARGET}.pch)
        set(PCH_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}.dir/$<CONFIG>/${PCH_HEADER_NAME}.pch)
        
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
            elseif (CLANG) # Clang
                # set_source_files_properties(${FILE} PROPERTIES COMPILER_FLAGS "-include ${CMAKE_CURRENT_SOURCE_DIR}/${PCH_HEADER}")
            endif()
        endif()
    endforeach()

    # add PCH_HEADER to TARGET includes
    if (MSVC)
        target_compile_options(${TARGET} PRIVATE "/FI${PCH_HEADER_NAME}")
    elseif (CLANG) # Clang
        target_compile_options(${TARGET} PRIVATE -include ${CMAKE_CURRENT_SOURCE_DIR}/${PCH_HEADER})
    endif()
endfunction()