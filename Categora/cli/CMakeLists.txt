# Gather CLI-specific sources
set(CLI_SOURCES
    main.cpp
    envloader.cpp
)

set(CLI_HEADERS
    envloader.h
)

# Create the CLI executable
add_executable(CategoraCLI
    ${CLI_SOURCES}
    ${CLI_HEADERS}
)

# Link business logic and Qt Core
target_link_libraries(CategoraCLI PRIVATE
    BusinessLogic
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Network
    Qt${QT_VERSION_MAJOR}::Sql
)

# Enable automoc if CLI uses Qt signals/slots
# qt5_wrap_cpp(CLI_MOC_SRCS ${CLI_HEADERS})
target_sources(CategoraCLI PRIVATE ${CLI_MOC_SRCS})
