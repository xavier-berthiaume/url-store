# Gather all business logic sources/headers
set(BUSINESS_LOGIC_SOURCES
    "api/apimanager.cpp"
    "api/deepseek/deepseekapimanager.cpp"

    "db/dbmanager.cpp"

    "models/url/url.cpp"
)

set(BUSINESS_LOGIC_HEADERS
    "api/apimanager.h"
    "api/deepseek/deepseekapimanager.h"

    "db/dbmanager.h"

    "models/url/url.h"
)

# Create a static/shared library for business logic
add_library(BusinessLogic STATIC
    ${BUSINESS_LOGIC_SOURCES}
    ${BUSINESS_LOGIC_HEADERS}
)

target_link_libraries(BusinessLogic PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
