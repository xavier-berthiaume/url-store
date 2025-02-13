set(NETWORK_SOURCES
    "api/apimanager.cpp"
    "api/deepseek/deepseekapimanager.cpp"
    "server.cpp"
)

set(NETWORK_HEADERS
    "api/apimanager.h"
    "api/deepseek/deepseekapimanager.h"
    "server.h"
)

target_sources(BusinessLogic PRIVATE
    ${NETWORK_SOURCES}
    ${NETWORK_HEADERS}
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
