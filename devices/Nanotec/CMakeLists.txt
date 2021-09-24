add_library(TkModLabNanotec SHARED
        NanotecComHandler.cpp
        VNanotecSMCI36.cpp
        NanotecSMCI36Fake.cpp
        NanotecSMCI36.cpp
)

set_target_properties(TkModLabNanotec PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabNanotec PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabNanotec_test test.cc)
target_link_libraries(TkModLabNanotec_test LINK_PUBLIC TkModLabNanotec)
set_target_properties(TkModLabNanotec_test PROPERTIES OUTPUT_NAME test)