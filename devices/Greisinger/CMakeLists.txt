add_library(TkModLabGreisinger SHARED
        GMH3750ComHandler.cpp
        VGMH3750.cpp
        GMH3750Fake.cpp
        GMH3750.cpp
)

set_target_properties(TkModLabGreisinger PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabGreisinger PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)
