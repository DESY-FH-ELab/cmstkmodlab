set(CMAKE_AUTOMOC OFF)

include_directories(${PROJECT_SOURCE_DIR}/devices/Pfeiffer)

add_executable(ReadTPG262 ReadTPG262.cpp)
target_link_libraries(ReadTPG262 LINK_PUBLIC TkModLabPfeiffer)

INSTALL(
    TARGETS ReadTPG262
    EXPORT Tools
    RUNTIME DESTINATION bin
    ARCHIVE DESTINATION lib
)
