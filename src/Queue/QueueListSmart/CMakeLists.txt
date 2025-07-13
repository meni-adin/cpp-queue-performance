set(TARGET_NAME mdn_QueueListSmart)

set(TARGET_SOURCES
    "include/mdn/QueueListSmart.hpp"
)

add_library(${TARGET_NAME}
    INTERFACE
)

target_include_directories(${TARGET_NAME}
    INTERFACE "include"
    INTERFACE "../include"
)
