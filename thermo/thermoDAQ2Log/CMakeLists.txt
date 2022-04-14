set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)

add_executable(thermoDAQ2Log
        main.cc
        ThermoDAQStreamReader.cc
        ThermoDAQ2StreamReader.cc
)

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgts_INCLUDE_DIRS})
include_directories(${Qt5Script_INCLUDE_DIRS})
include_directories(${Qt5Svg_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgts_DEFINITIONS})
add_definitions(${Qt5Script_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgts_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Script_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")

include_directories(${PROJECT_SOURCE_DIR})

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/common)

target_link_libraries(thermoDAQ2Log
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Common
)

INSTALL(
        TARGETS thermoDAQ2Log
        EXPORT Thermo
        RUNTIME DESTINATION bin
        ARCHIVE DESTINATION lib
)
