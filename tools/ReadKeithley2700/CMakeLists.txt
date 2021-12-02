set(CMAKE_AUTOMOC OFF)

include_directories(${PROJECT_SOURCE_DIR}/devices/Keithley)

add_executable(ReadKeithley2700 ReadKeithley2700.cpp)
target_link_libraries(ReadKeithley2700 LINK_PUBLIC TkModLabKeithley)

INSTALL(
    TARGETS ReadKeithley2700
    EXPORT Tools
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
