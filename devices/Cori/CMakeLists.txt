add_library(TkModLabCori SHARED
        CoriFlowComHandler.cpp
        VCoriFlow.cpp
        CoriFlowFake.cpp
        CoriFlow.cpp
)

set_target_properties(TkModLabCori PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabCori PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabCori_test test.cc)
target_link_libraries(TkModLabCori_test LINK_PUBLIC TkModLabCori)
set_target_properties(TkModLabCori_test PROPERTIES OUTPUT_NAME test)