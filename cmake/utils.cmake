function(${PROJECT_NAME}_print_variable variable)
    if(DEFINED ${variable})
        message(STATUS "${variable} = ${${variable}}")
    else()
        message(FATAL_ERROR "Variable '${variable}' is not defined.")
    endif()
endfunction()

function(${PROJECT_NAME}_set_compiler_flags)
    if((CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
        add_compile_options(
            -Werror
            -Wall
            -Wextra
            -Wpedantic
            -Wvla
            -Wformat
            # -Wshadow  # turned off as it may be useful in macros
            -Wconversion
            -Wnull-dereference
            -Wdouble-promotion
            -Wimplicit-fallthrough
            -Wno-switch
        )
        if(${PROJECT_NAME_UC}_ENABLE_COVERAGE)
            add_compile_options(
                -fprofile-arcs
                -ftest-coverage
                )
            add_link_options(
                -fprofile-arcs
                -ftest-coverage
            )
        endif()
        if(CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang")
        endif()
        if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        endif()
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(
            /Wall
            /WX
            /wd4061
            /wd4062
            /wd4710
            /wd4711
            /wd4820
            /wd5045
            /wd5072
        )
    else()
        message(FATAL_ERROR "Unknown CXX compiler: ${CMAKE_CXX_COMPILER_ID}")
    endif()
endfunction()

function(${PROJECT_NAME}_enable_sanitizers)
    if((CMAKE_CXX_COMPILER_ID STREQUAL "AppleClang") OR (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
        add_compile_options(-fsanitize=address -fno-omit-frame-pointer)
        add_link_options(-fsanitize=address)
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        add_compile_options(/fsanitize=address)
        add_link_options(/fsanitize=address)

        foreach(flag_var
            CMAKE_CXX_FLAGS
            CMAKE_CXX_FLAGS_DEBUG
            CMAKE_CXX_FLAGS_RELEASE
            CMAKE_CXX_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS_MINSIZEREL)
            if(${flag_var} MATCHES "/MT")
                string(REPLACE "/MT" "/MD" ${flag_var} "${${flag_var}}")
            endif()
            if(${flag_var} MATCHES "/MTd")
                string(REPLACE "/MTd" "/MDd" ${flag_var} "${${flag_var}}")
            endif()
        endforeach()
    endif()
endfunction()

function(${PROJECT_NAME}_delete_gcda_files)
    message(STATUS "Deleting all '.gcda' files in the build directory, to enable regeneration of coverage related files")

    file(GLOB_RECURSE GCDA_FILES "${CMAKE_CURRENT_BINARY_DIR}/*.gcda")
    foreach(GCDA_FILE ${GCDA_FILES})
        # Note: currently also deleting files in the 'external' directory
        message(STATUS "Deleting file: ${GCDA_FILE}")
        file(REMOVE ${GCDA_FILE})
    endforeach()

    message(STATUS "Done Removing 'CMakeFiles' directories")
endfunction()

function(${PROJECT_NAME}_setup_tests_data TARGET_NAME)
    string(TOUPPER ${TARGET_NAME} TARGET_NAME_UC)

    set(TEST_DATA_FILE_NAME "tests_data.json")
    file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${TEST_DATA_FILE_NAME}" TEST_DATA_SOURCE_PATH)
    file(TO_CMAKE_PATH "${CMAKE_CURRENT_BINARY_DIR}/${TEST_DATA_FILE_NAME}" TEST_DATA_DEST_PATH)
    add_custom_command(
        OUTPUT ${TEST_DATA_DEST_PATH}
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${TEST_DATA_SOURCE_PATH}
            ${TEST_DATA_DEST_PATH}
        DEPENDS ${TEST_DATA_SOURCE_PATH}
        COMMENT "Copying test data file"
    )
    add_custom_target(${TARGET_NAME}_copy_tests_data ALL DEPENDS ${TEST_DATA_DEST_PATH})
    add_dependencies(${TARGET_NAME} ${TARGET_NAME}_copy_tests_data)

    target_compile_definitions(${TARGET_NAME}
        PRIVATE ${TARGET_NAME_UC}_TESTS_DATA_FILE_PATH="${TEST_DATA_DEST_PATH}"
    )
endfunction()
