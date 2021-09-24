add_library(TkModLabKeyence SHARED
        KeyenceComHandler.cpp
        VKeyence.cpp
        Keyence.cpp
        KeyenceFake.cpp
)

set_target_properties(TkModLabKeyence PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabKeyence PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabKeyence_test test.cc)
target_link_libraries(TkModLabKeyence_test LINK_PUBLIC TkModLabKeyence)
set_target_properties(TkModLabKeyence_test PROPERTIES OUTPUT_NAME test)