set(MODEL_SOURCES
    "url/url.cpp"
    "url/qturlwrapper.cpp"
    "token/token.cpp"
    "token/qttokenwrapper.cpp"
)

set(MODEL_HEADERS
    "url/url.h"
    "url/qturlwrapper.h"
    "token/token.h"
    "token/qttokenwrapper.h"
)

target_sources(BusinessLogic PRIVATE
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
