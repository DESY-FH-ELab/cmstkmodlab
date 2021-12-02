set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

add_executable(defoCalib
        main.cpp
)

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
        add_compile_definitions(APPLICATIONVERSIONSTR=\"unknown\")
else()
        execute_process(COMMAND bash -c "git describe --dirty --always --tags" OUTPUT_VARIABLE GIT_DESCRIBE)
	string(REGEX REPLACE "\n$" "" GIT_DESCRIBE "${GIT_DESCRIBE}")
        add_compile_definitions(APPLICATIONVERSIONSTR=\"${GIT_DESCRIBE}\")
endif()

add_compile_definitions(__${CMAKE_HOST_SYSTEM_NAME}__)

include_directories(${PROJECT_SOURCE_DIR})

include_directories(${Qt5Core_INCLUDE_DIRS})
include_directories(${Qt5Widgts_INCLUDE_DIRS})
include_directories(${Qt5Script_INCLUDE_DIRS})
include_directories(${Qt5Svg_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})

include_directories(${QWT_INCLUDE_DIR})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgts_DEFINITIONS})
add_definitions(${Qt5Script_DEFINITIONS})
add_definitions(${Qt5Svg_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgts_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Script_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Svg_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/defo/defoCommon)

target_link_libraries(defoCalib
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::Script
        PRIVATE Qt5::Svg
        PRIVATE Qt5::Network
        PRIVATE Common
        PRIVATE defoCommon
        PUBLIC ${QWT_LIBRARY}
)

INSTALL(
    TARGETS defoCalib
    EXPORT Defo
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
