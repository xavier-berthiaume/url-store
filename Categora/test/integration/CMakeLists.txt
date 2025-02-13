# tests/unit/CMakeLists.txt
# Gather unit test sources
file(GLOB UNIT_TEST_SOURCES
    "*.cpp"
)

# Create an executable for unit tests
add_executable(integration_tests
    ${UNIT_TEST_SOURCES}
)

# Link to Google Test and your business logic
target_link_libraries(integration_tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    BusinessLogic
)

# Add the test to CMake's test suite
add_test(NAME integration_tests COMMAND integration_tests)
