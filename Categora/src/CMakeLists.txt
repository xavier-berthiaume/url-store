# Create a static/shared library for business logic
add_library(BusinessLogic STATIC)

target_link_libraries(BusinessLogic PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

target_include_directories(BusinessLogic PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(network)
add_subdirectory(db)
add_subdirectory(models)
