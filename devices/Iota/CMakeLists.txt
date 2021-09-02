add_library(TkModLabIota SHARED
        Iota300ComHandler.cpp
        VIota300.cpp
        Iota300Fake.cpp
        Iota300.cpp
)

set_target_properties(TkModLabIota PROPERTIES VERSION ${PROJECT_VERSION})
set_target_properties(TkModLabIota PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/devices/lib)

add_executable(TkModLabIota_test test.cc)
target_link_libraries(TkModLabIota_test LINK_PUBLIC TkModLabIota)
set_target_properties(TkModLabIota_test PROPERTIES OUTPUT_NAME test)