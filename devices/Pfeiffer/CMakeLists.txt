add_library(TkModLabPfeiffer SHARED
        TPG262ComHandler.cpp
        VPfeifferTPG262.cpp
        PfeifferTPG262Fake.cpp
        PfeifferTPG262.cpp
)

set_target_properties(TkModLabPfeiffer PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabPfeiffer PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabPfeiffer_test test.cc)
target_link_libraries(TkModLabPfeiffer_test LINK_PUBLIC TkModLabPfeiffer)
set_target_properties(TkModLabPfeiffer_test PROPERTIES OUTPUT_NAME test)