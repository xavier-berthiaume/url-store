set(MODEL_SOURCES
    "url/url.cpp"
    "token/token.cpp"
)

set(MODEL_HEADERS
    "url/url.h"
    "token/token.h"
)

target_sources(BusinessLogic PRIVATE
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
