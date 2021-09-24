add_library(TkModLabVelleman SHARED
        VellemanCommunication.cpp
        VVellemanController.cpp
        VellemanController.cpp
        VellemanControllerFake.cpp
)

set_target_properties(TkModLabVelleman PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabVelleman PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabVelleman_test vellemanTest.cpp)
target_link_libraries(TkModLabVelleman_test LINK_PUBLIC TkModLabVelleman)
set_target_properties(TkModLabVelleman_test PROPERTIES OUTPUT_NAME vellemanTest)
