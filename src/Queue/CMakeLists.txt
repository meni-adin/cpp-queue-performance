set(TARGET_NAME mdn_Queue)

set(TARGET_SOURCES
    "include/mdn/Queue.hpp"
)

add_library(${TARGET_NAME}
    INTERFACE
)

target_include_directories(${TARGET_NAME}
    INTERFACE "include"
)

add_subdirectory(QueueListRaw)
add_subdirectory(QueueListSmart)
add_subdirectory(QueueVector)
