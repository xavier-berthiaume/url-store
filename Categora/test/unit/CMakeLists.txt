# tests/unit/CMakeLists.txt
# Gather unit test sources
set(UNIT_TEST_SOURCES
    "deepseekapitests.cpp"
    "mocknetworkreply.h"
    "mocknetworkaccessmanager.h"
)

# Create an executable for unit tests
add_executable(unit_tests
    ${UNIT_TEST_SOURCES}
)

# Link to Google Test and your business logic
target_link_libraries(unit_tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Network
    BusinessLogic
)

# Add the test to CMake's test suite
add_test(NAME unit_tests COMMAND unit_tests)
