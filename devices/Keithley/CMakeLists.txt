add_library(TkModLabKeithley SHARED
        KMMComHandler.cpp
        VKeithley2700.cpp
        Keithley2700Fake.cpp
        Keithley2700.cpp
        KeithleyUSBTMCComHandler.cpp
        VKeithleyDAQ6510.cpp
        KeithleyDAQ6510Fake.cpp
        KeithleyDAQ6510.cpp
)

set_target_properties(TkModLabKeithley PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabKeithley PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabKeithley_test test.cc)
target_link_libraries(TkModLabKeithley_test LINK_PUBLIC TkModLabKeithley)
set_target_properties(TkModLabKeithley_test PROPERTIES OUTPUT_NAME test)