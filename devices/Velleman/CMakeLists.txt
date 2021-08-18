add_library(TkModLabVellemann SHARED
        VellemanCommunication.cpp
        VVellemanController.cpp
        VellemanController.cpp
        VellemanControllerFake.cpp
)

set_target_properties(TkModLabVellemann PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabVellemann PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabVellemann_test vellemanTest.cpp)
target_link_libraries(TkModLabVellemann_test LINK_PUBLIC TkModLabVellemann)
set_target_properties(TkModLabVellemann_test PROPERTIES OUTPUT_NAME vellemanTest)