set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOMOC ON)

add_executable(PumpStationDaemon
        PumpStationDaemon.cc
        PumpStationModel.cc
        CommunicationThread.cc
        CommunicationServer.cc
        DataLogger.cc
        WatchDog.cc
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
include_directories(${Qt5Network_INCLUDE_DIRS})

add_definitions(${Qt5Core_DEFINITIONS})
add_definitions(${Qt5Network_DEFINITIONS})

set(CMAKE_CXX_FLAGS "${Qt5Core_EXECUTABLE_COMPILE_FLAGS}")
set(CMAKE_CXX_FLAGS "${Qt5Network_EXECUTABLE_COMPILE_FLAGS}")

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/common)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/common)

target_link_libraries(PumpStationDaemon
        PRIVATE Qt5::Core
        PRIVATE Qt5::Widgets
        PRIVATE Qt5::Script
        PRIVATE Qt5::Svg
        PRIVATE Qt5::Network
        PRIVATE Common
)

configure_file(pumpstation.service.in pumpstation.service)

add_custom_target(restart
        COMMAND ${CMAKE_COMMAND} --build . --target install; systemctl stop pumpstation; chmod 664 pumpstation.service; cp pumpstation.service /etc/systemd/system; systemctl daemon-reload; systemctl enable pumpstation; systemctl start pumpstation
        COMMENT "restart pump station daemon ..."
)

INSTALL(
    TARGETS PumpStationDaemon
    EXPORT Pumpstation
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
