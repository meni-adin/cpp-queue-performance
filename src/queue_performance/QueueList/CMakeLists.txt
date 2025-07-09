set(TARGET_NAME mdn_QueueList)

set(TARGET_SOURCES
    "QueueList.cpp"
)

add_library(${TARGET_NAME} STATIC
    ${TARGET_SOURCES}
)

target_include_directories(${TARGET_NAME}
    PUBLIC "include"
    PUBLIC "../include"
)
