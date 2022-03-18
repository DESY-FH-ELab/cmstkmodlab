set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

add_executable(PumpStationControl
        PumpStationControl.cc
        Controller.cc
        ${PROJECT_SOURCE_DIR}/common/ApplicationConfig.cc
        ${PROJECT_SOURCE_DIR}/common/ApplicationConfigReader.cc
        ${PROJECT_SOURCE_DIR}/common/ApplicationConfigWriter.cc
        ${PROJECT_SOURCE_DIR}/common/nqlogger.cc
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
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${Qt5Network_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Widgets_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++17")

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/common)

target_compile_features(PumpStationControl PRIVATE cxx_std_17)

target_link_libraries(PumpStationControl
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::Network
	Common
)

INSTALL(
    TARGETS PumpStationControl
    EXPORT Pumpstation
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
