add_library(TkModLabAgilent SHARED
        AgilentTwisTorr304ComHandler.cpp
        VAgilentTwisTorr304.cpp
        AgilentTwisTorr304Fake.cpp
        AgilentTwisTorr304.cpp
)

set_target_properties(TkModLabAgilent PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabAgilent PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabAgilent_test test.cc)
target_link_libraries(TkModLabAgilent_test LINK_PUBLIC TkModLabAgilent)
set_target_properties(TkModLabAgilent_test PROPERTIES OUTPUT_NAME test)
