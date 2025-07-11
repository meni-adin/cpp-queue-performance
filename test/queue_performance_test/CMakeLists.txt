set(TARGET_NAME mdn_queue_performance_test)

include(GoogleTest)

set(TARGET_SOURCES
    "queue_performance_test.cpp"
)

add_executable(${TARGET_NAME}
    ${TARGET_SOURCES}
)

target_link_libraries(${TARGET_NAME}
    GTest::gmock
    mdn_Queue
    mdn_QueueList
    mdn_QueueVector
)

gtest_discover_tests(${TARGET_NAME})
