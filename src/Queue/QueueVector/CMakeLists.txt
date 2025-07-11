set(TARGET_NAME mdn_QueueVector)

set(TARGET_SOURCES
    "include/mdn/QueueVector.hpp"
)

add_library(${TARGET_NAME}
    INTERFACE
)

target_include_directories(${TARGET_NAME}
    INTERFACE "include"
    INTERFACE "../include"
)
