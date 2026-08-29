cmake_minimum_required(VERSION 3.16)
project(TitleCaseConverter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 without fixed paths
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Source files
set(SOURCES
    main.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
)

# Executable file
add_executable(title_case_converter ${SOURCES} ${HEADERS})

# Link Qt Libs
target_link_libraries(title_case_converter
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(title_case_converter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Qt MOC
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
target_sources(title_case_converter PRIVATE ${MOC_SOURCES})

# For MacOS
if(APPLE)
    set_target_properties(title_case_converter PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()
