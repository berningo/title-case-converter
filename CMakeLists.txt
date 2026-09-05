cmake_minimum_required(VERSION 3.16)
project(TitleCaseConverter VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
find_package(wxWidgets 3.2 REQUIRED COMPONENTS core base)
include(${wxWidgets_USE_FILE})

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

target_link_libraries(title_case_converter PRIVATE ${wxWidgets_LIBRARIES})

target_include_directories(title_case_converter PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# For MacOS
if(APPLE)
    set_target_properties(title_case_converter PROPERTIES
        MACOSX_BUNDLE TRUE
    )
endif()
