add_library(TkModLabConrad SHARED
        ConradCommunication.cpp
        VConradController.cpp
        ConradController.cpp
        ConradControllerFake.cpp
)

set_target_properties(TkModLabConrad PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabConrad PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)
