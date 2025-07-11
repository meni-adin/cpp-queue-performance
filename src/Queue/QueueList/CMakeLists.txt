set(TARGET_NAME mdn_QueueList)

set(TARGET_SOURCES
    "include/mdn/QueueList.hpp"
)

add_library(${TARGET_NAME}
    INTERFACE
)

target_include_directories(${TARGET_NAME}
    INTERFACE "include"
    INTERFACE "../include"
)
