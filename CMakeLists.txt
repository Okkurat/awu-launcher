cmake_minimum_required(VERSION 3.5)

project(awu)

set(CMAKE_CXX_STANDARD 11)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
QT5_WRAP_CPP(MOC_SOURCES appedit.h)


set(SOURCES
    main.cpp
    appedit.cpp
    ${MOC_SOURCES}
)

set(HEADERS
    appedit.h
)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

install(TARGETS ${PROJECT_NAME} DESTINATION /usr/bin)
