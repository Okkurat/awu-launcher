cmake_minimum_required(VERSION 3.5)

project(awu)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC ON)

set(SOURCES
    src/main.cpp
    src/app_edit.cpp
    src/main_window.cpp
    src/utils.cpp
)

set(HEADERS
    include/app_edit.h
    include/main_window.h
    include/utils.h
)

include_directories(include src)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
