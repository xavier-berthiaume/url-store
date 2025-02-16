set(DB_SOURCES
    "abstractdbmanager.cpp"
    "sqlitedbmanager.cpp"
)

set(DB_HEADERS
    "abstractdbmanager.h"
    "sqlitedbmanager.h"
)

target_sources(BusinessLogic PRIVATE
    ${DB_SOURCES}
    ${DB_HEADERS}
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
