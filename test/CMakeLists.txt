include(fetch_content_wrapper)
cmake_language(CALL ${PROJECT_NAME}_fetch_content_wrapper
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        origin/main
)
target_compile_options(gtest PRIVATE -Wno-null-dereference)

enable_testing()

add_subdirectory(queue_performance_test)
