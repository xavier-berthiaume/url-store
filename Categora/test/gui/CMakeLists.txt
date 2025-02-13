# tests/unit/CMakeLists.txt
# Gather unit test sources
file(GLOB UNIT_TEST_SOURCES
    "*.cpp"
)

# Create an executable for unit tests
add_executable(gui_tests
    ${UNIT_TEST_SOURCES}
)

# Link to Google Test and your business logic
target_link_libraries(gui_tests PRIVATE
    Qt${QT_VERSION_MAJOR}::Test
    BusinessLogic
)

# Add the test to CMake's test suite
add_test(NAME gui_tests COMMAND gui_tests)
