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

target_link_libraries(${TARGET_NAME}
    INTERFACE mdn_QueueList
)

add_subdirectory(QueueList)
add_subdirectory(QueueVector)
