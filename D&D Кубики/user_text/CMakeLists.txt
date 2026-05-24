cmake_minimum_required(VERSION 3.16)
project(UserText LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)

set(CMAKE_PREFIX_PATH "C:/Qt/6.8.3/mingw_64" CACHE PATH "Qt prefix")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
qt_standard_project_setup()

qt_add_executable(UserText
    src/main.cpp
)

target_link_libraries(UserText PRIVATE Qt6::Widgets)

if(WIN32)
    set_target_properties(UserText PROPERTIES WIN32_EXECUTABLE TRUE)
endif()
