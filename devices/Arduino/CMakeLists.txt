add_library(TkModLabArduino SHARED
        ArduinoComHandler.cpp
        VArduinoPres.cpp
        ArduinoPresFake.cpp
        ArduinoPres.cpp
        ArduinoMotor.cpp
        VArduinoMotor.cpp
)

set_target_properties(TkModLabArduino PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabArduino PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabArduino_testPres testPres.cc)
target_link_libraries(TkModLabArduino_testPres LINK_PUBLIC TkModLabArduino)
set_target_properties(TkModLabArduino_testPres PROPERTIES OUTPUT_NAME testPres)
