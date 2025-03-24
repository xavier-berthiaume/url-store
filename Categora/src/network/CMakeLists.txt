set(NETWORK_SOURCES
    "api/apimanager.cpp"
    "api/deepseek/deepseekapimanager.cpp"
    
    "server/tcpserver.cpp"
    "server/abstractserver.cpp"
    "server/httpserver.cpp"
)

set(NETWORK_HEADERS
    "api/apimanager.h"
    "api/deepseek/deepseekapimanager.h"

    "server/tcpserver.h"
    "server/abstractserver.h"
    "server/httpserver.h"
)

target_sources(BusinessLogic PRIVATE
    ${NETWORK_SOURCES}
    ${NETWORK_HEADERS}
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
