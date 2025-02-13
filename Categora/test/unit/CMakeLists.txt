set(UNIT_TEST_SOURCES
    "testtcpserver.cpp"
)

add_executable(test_tcp_server
    "testtcpserver.cpp"
)

add_executable(test_deepseek_api_manager
    "deepseekapitests.cpp"
    "mocknetworkreply.h"
    "mocknetworkaccessmanager.h"
)

target_link_libraries(test_tcp_server PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Network
    BusinessLogic
)

target_link_libraries(test_deepseek_api_manager PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Test
    Qt${QT_VERSION_MAJOR}::Network
    BusinessLogic
)

add_test(NAME test_tcp_server COMMAND test_tcp_server)
add_test(NAME test_deepseek_api_manager COMMAND test_deepseek_api_manager)
