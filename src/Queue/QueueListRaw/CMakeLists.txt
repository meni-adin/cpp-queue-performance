set(TARGET_NAME mdn_QueueListRaw)

set(TARGET_SOURCES
    "include/mdn/QueueListRaw.hpp"
)

add_library(${TARGET_NAME}
    INTERFACE
)

target_include_directories(${TARGET_NAME}
    INTERFACE "include"
    INTERFACE "../include"
)
