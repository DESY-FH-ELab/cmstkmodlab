set(CMAKE_AUTOMOC OFF)

include_directories(${PROJECT_SOURCE_DIR}/devices/Greisinger)

add_executable(ReadGMH3750 ReadGMH3750.cpp)
target_link_libraries(ReadGMH3750 LINK_PUBLIC TkModLabGreisinger)

INSTALL(
    TARGETS ReadGMH3750
    EXPORT Tools
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
