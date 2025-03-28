cmake_minimum_required(VERSION 3.16)
project(Categora LANGUAGES CXX)

# Set C++ standard
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt packages
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets LinguistTools Network HttpServer Sql Test)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets LinguistTools Network HttpServer Sql Test)

# Copy .env file to build directory
configure_file(
    ${CMAKE_SOURCE_DIR}/.env
    ${CMAKE_BINARY_DIR}/.env
    COPYONLY
)

# Include subdirectories
add_subdirectory(src)
add_subdirectory(gui)
add_subdirectory(cli)
add_subdirectory(test)
