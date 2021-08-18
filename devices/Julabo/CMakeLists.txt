add_library(TkModLabJulabo SHARED
        FP50ComHandler.cpp
        VJulaboFP50.cpp
        JulaboFP50Fake.cpp
        JulaboFP50.cpp
)

set_target_properties(TkModLabJulabo PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabJulabo PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)
