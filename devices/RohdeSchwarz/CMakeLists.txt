add_library(TkModLabRohdeSchwarz SHARED
        NGE103BComHandler.cpp
        VRohdeSchwarzNGE103B.cpp
        RohdeSchwarzNGE103BFake.cpp
        RohdeSchwarzNGE103B.cpp
)

set_target_properties(TkModLabRohdeSchwarz PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabRohdeSchwarz PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabRohdeSchwarz_test test.cc)
target_link_libraries(TkModLabRohdeSchwarz_test LINK_PUBLIC TkModLabRohdeSchwarz)
set_target_properties(TkModLabRohdeSchwarz_test PROPERTIES OUTPUT_NAME test)
