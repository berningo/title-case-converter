cmake_minimum_required(VERSION 3.16)
project(TitleCaseConverter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt6 finden - OHNE feste Pfade!
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Quellcodedateien
set(SOURCES
    main.cpp
    mainwindow.cpp
)

set(HEADERS
    mainwindow.h
)

# Ausführbare Datei
add_executable(title_case_converter ${SOURCES} ${HEADERS})

# Qt-Bibliotheken verknüpfen
target_link_libraries(title_case_converter
    Qt6::Core
    Qt6::Widgets
)

target_include_directories(title_case_converter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Qt-MOC
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})
target_sources(title_case_converter PRIVATE ${MOC_SOURCES})

# macOS-spezifisch
if(APPLE)
    set_target_properties(title_case_converter PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()
