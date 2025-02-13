set(DB_SOURCES
    "dbmanager.cpp"
)

set(DB_HEADERS
    "dbmanager.h"
)

target_sources(BusinessLogic PRIVATE
    ${DB_SOURCES}
    ${DB_HEADERS}
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
