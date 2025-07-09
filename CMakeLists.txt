cmake_minimum_required(VERSION 3.28)
project(mdn_queue_performance
    LANGUAGES CXX
)
string(TOUPPER ${PROJECT_NAME} PROJECT_NAME_UC)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(TO_CMAKE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake" MY_CMAKE_SCRIPTS_DIR)
set(CMAKE_MODULE_PATH "${MY_CMAKE_SCRIPTS_DIR}" ${CMAKE_MODULE_PATH})

include(utils)
include(fetch_content_wrapper)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(${PROJECT_NAME_UC}_ENABLE_COVERAGE ON)
    cmake_language(CALL ${PROJECT_NAME}_delete_gcda_files)
else()
    set(${PROJECT_NAME_UC}_ENABLE_COVERAGE OFF)
endif()

cmake_language(CALL ${PROJECT_NAME}_init_fetch_content_wrapper)
cmake_language(CALL ${PROJECT_NAME}_set_compiler_flags)

cmake_language(CALL ${PROJECT_NAME}_print_variable CMAKE_CXX_COMPILER_ID)
cmake_language(CALL ${PROJECT_NAME}_print_variable ${PROJECT_NAME_UC}_SAFE_MODE)
cmake_language(CALL ${PROJECT_NAME}_print_variable ${PROJECT_NAME_UC}_SANITIZED_BUILD)
cmake_language(CALL ${PROJECT_NAME}_print_variable ${PROJECT_NAME_UC}_ENABLE_COVERAGE)

if(${PROJECT_NAME_UC}_SANITIZED_BUILD)
    cmake_language(CALL ${PROJECT_NAME}_enable_sanitizers)
endif()

include(CTest)

add_subdirectory(src)
add_subdirectory(test)
