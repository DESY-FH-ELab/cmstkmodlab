add_library(TkModLabLeybold SHARED
        LeyboldComHandler.cpp
        VLeyboldGraphix.cpp
        VLeyboldGraphixOne.cpp
        LeyboldGraphixOne.cpp
        LeyboldGraphixOneFake.cpp
        VLeyboldGraphixThree.cpp
        LeyboldGraphixThree.cpp
        LeyboldGraphixThreeFake.cpp
)

set_target_properties(TkModLabLeybold PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabLeybold PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabLeybold_test test.cc)
target_link_libraries(TkModLabLeybold_test LINK_PUBLIC TkModLabLeybold)
set_target_properties(TkModLabLeybold_test PROPERTIES OUTPUT_NAME test)