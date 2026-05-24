cmake_minimum_required(VERSION 3.16)
project(DndDice LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.3/mingw_64" CACHE PATH "Qt prefix")

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(DndDice
    src/qt/main.cpp
    src/qt/MainWindow.cpp
    src/qt/MainWindow.h
    src/qt/DieIconButton.cpp
    src/qt/DieIconButton.h
    src/DiceRoller.cpp
    src/DiceRoller.h
    resources/app.qrc
)

target_include_directories(DndDice PRIVATE src)
target_link_libraries(DndDice PRIVATE Qt6::Widgets)

if(WIN32)
    set_target_properties(DndDice PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
