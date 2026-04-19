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
    if (MSVC)
        # extract pch filename
        get_filename_component(PCH_HEADER_NAME ${PCH_HEADER} NAME)

        # create by /Yc .pch file from PCH_SOURCE
        target_sources(${TARGET} PRIVATE ${PCH_HEADER} ${PCH_SOURCE})
        set_source_files_properties(${PCH_SOURCE} PROPERTIES COMPILE_FLAGS "/Yc${PCH_HEADER_NAME}")

        # mark all other .cpp (except PCH_SOURCE) to use .pch file
        foreach(FILE ${SOURCE_FILES})
            if (${FILE} MATCHES "\\.cpp$" AND NOT ${FILE} STREQUAL ${PCH_SOURCE})
                set_source_files_properties(${FILE} PROPERTIES COMPILE_FLAGS "/Yu${PCH_HEADER_NAME}")
            endif()
        endforeach()

        # add PCH_HEADER to TARGET includes
        target_compile_options(${TARGET} PRIVATE "/FI${PCH_HEADER_NAME}")
    endif()
endfunction()